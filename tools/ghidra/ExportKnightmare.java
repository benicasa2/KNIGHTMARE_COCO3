import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.nio.file.*;

/** Export observations; automatic function boundaries are not original symbols. */
public class ExportKnightmare extends GhidraScript {
 public void run() throws Exception {
  Path out=Path.of(getScriptArgs()[0]); Files.createDirectories(out);
  try(PrintWriter w=new PrintWriter(out.resolve("listing-z80.tsv").toFile())) {
   w.println("address\tbytes\tassembly\tflow");
   for(Instruction i: currentProgram.getListing().getInstructions(true)) {
    StringBuilder h=new StringBuilder(); for(byte b:i.getBytes()) h.append(String.format("%02x", b&255));
    w.println(i.getAddress()+"\t"+h+"\t"+i+"\t"+i.getFlowType());
   }
  }
  DecompInterface di=new DecompInterface(); di.openProgram(currentProgram);
  try(PrintWriter w=new PrintWriter(out.resolve("functions.tsv").toFile());
      PrintWriter c=new PrintWriter(out.resolve("decompiled-auto.c").toFile());
      PrintWriter r=new PrintWriter(out.resolve("references.tsv").toFile())) {
   w.println("entry\tname\tbody_bytes"); r.println("from\tto\ttype");
   for(Function f: currentProgram.getFunctionManager().getFunctions(true)) {
    w.println(f.getEntryPoint()+"\t"+f.getName()+"\t"+f.getBody().getNumAddresses());
    DecompileResults d=di.decompileFunction(f,15,monitor);
    c.println("\n/* "+f.getEntryPoint()+" "+f.getName()+" -- automatic, unverified */");
    if(d.decompileCompleted()) c.println(d.getDecompiledFunction().getC()); else c.println("/* "+d.getErrorMessage()+" */");
   }
   for(Instruction i:currentProgram.getListing().getInstructions(true))
    for(Reference ref:i.getReferencesFrom()) r.println(ref.getFromAddress()+"\t"+ref.getToAddress()+"\t"+ref.getReferenceType());
  } finally {di.dispose();}
  println("KNIGHTMARE_EXPORT_OK functions="+currentProgram.getFunctionManager().getFunctionCount());
 }
}
