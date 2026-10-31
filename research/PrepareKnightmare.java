import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
public class PrepareKnightmare extends GhidraScript {
 public void run() throws Exception {
  // Inline dispatcher pops the CALL return address as a table pointer.
  // Execution continues at the selected target, never at the table bytes.
  disassemble(toAddr(0x406c)); Function f=getFunctionAt(toAddr(0x406c));
  if(f==null) f=createFunction(toAddr(0x406c),"inline_dispatch");
  if(f!=null) f.setNoReturn(true);
  addEntryPoint(toAddr(0x407e));addEntryPoint(toAddr(0x402e));
 }
}
