"""Real XRoar/6809 execution: Disk BASIC boot, framebuffer and timed DAC probes."""
from pathlib import Path
import argparse,subprocess,socket,time,re,json,hashlib,struct
ROOT=Path(__file__).resolve().parents[1]
class RSP:
 def __init__(self,port):
  self.s=socket.create_connection(('127.0.0.1',port),timeout=20);self.s.setsockopt(socket.IPPROTO_TCP,socket.TCP_NODELAY,1)
 def cmd(self,cmd):
  b=cmd.encode();self.s.sendall(b'$'+b+b'#'+f'{sum(b)&255:02x}'.encode())
  return self.recv()
 def recv(self):
  while True:
   c=self.s.recv(1)
   if c==b'$':break
   if not c:raise EOFError
  out=bytearray()
  while (c:=self.s.recv(1))!=b'#':
   if not c:raise EOFError
   out+=c
  chk=self.s.recv(2);assert int(chk,16)==sum(out)%256
  self.s.sendall(b'+');return out.decode()
 def read(self,a,n):return b''.join(bytes.fromhex(self.cmd(f'm{a+p:x},{min(512,n-p):x}')) for p in range(0,n,512))
 def write(self,a,b):assert self.cmd(f'M{a:x},{len(b):x}:'+b.hex())=='OK'
 def bp(self,a):assert self.cmd(f'Z0,{a:x},1')=='OK'
 def unbp(self,a):assert self.cmd(f'z0,{a:x},1')=='OK'
 def go(self):assert self.cmd('c')=='S05'
 def step(self):assert self.cmd('s')=='S05'
def symbols(name):
 return {m[1]:int(m[2],16) for m in re.finditer(r'(?m)^(\w+) EQU \$([0-9a-fA-F]+)',(ROOT/f'analysis/{name}.sym').read_text())}
def main():
 from verify_combat import main as verify_current
 verify_current()
if __name__=='__main__':main()
