
/* ram:4025 FUN_ram_4025 -- automatic, unverified */

void FUN_ram_4025(void)

{
  FUN_ram_46dd(0xc9e1);
  return;
}



/* ram:402e FUN_ram_402e -- automatic, unverified */

void FUN_ram_402e(void)

{
  char cVar1;
  
  func_0x013e();
  disableMaskableInterrupts();
  audio_tick();
  if ((DAT_ram_e005 & 1) == 0) {
    DAT_ram_e005 = DAT_ram_e005 + 1;
    enableMaskableInterrupts();
    FUN_ram_44d2();
    FUN_ram_40d3();
    DAT_ram_e005 = 0;
  }
  cVar1 = func_0x013e();
  disableMaskableInterrupts();
  if (cVar1 < '\0') {
    audio_tick(cVar1);
  }
  enableMaskableInterrupts();
  return;
}



/* ram:4052 FUN_ram_4052 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_4052(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:405b FUN_ram_405b -- automatic, unverified */

char FUN_ram_405b(byte param_1,byte param_2)

{
  if (!CARRY1(param_1,param_2)) {
    return param_1 + param_2;
  }
  return param_1 + param_2;
}



/* ram:4060 FUN_ram_4060 -- automatic, unverified */

char FUN_ram_4060(byte param_1,byte param_2)

{
  if (!CARRY1(param_1,param_2)) {
    return param_1 + param_2;
  }
  return param_1 + param_2;
}



/* ram:4065 FUN_ram_4065 -- automatic, unverified */

void FUN_ram_4065(undefined2 param_1,char *param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  
  uVar2 = (undefined1)((ushort)param_1 >> 8);
  uVar1 = (undefined1)param_1;
  if ((char)(*param_2 + -1) < '\0') {
    return;
  }
  FUN_ram_4079(*param_2 + -1);
                    /* WARNING: Could not recover jumptable at 0x4070. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)CONCAT11(uVar2,uVar1))();
  return;
}



/* ram:406c inline_dispatch -- automatic, unverified */

void inline_dispatch(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  FUN_ram_4079();
                    /* WARNING: Could not recover jumptable at 0x4070. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



/* ram:4071 FUN_ram_4071 -- automatic, unverified */

void FUN_ram_4071(char param_1)

{
  FUN_ram_405b(param_1 * '\x02');
  return;
}



/* ram:4072 FUN_ram_4072 -- automatic, unverified */

void FUN_ram_4072(void)

{
  FUN_ram_405b();
  return;
}



/* ram:4079 FUN_ram_4079 -- automatic, unverified */

void FUN_ram_4079(void)

{
  FUN_ram_4071();
  return;
}



/* ram:407e FUN_ram_407e -- automatic, unverified */

/* WARNING: This function may have set the stack pointer */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_407e(void)

{
  byte bVar1;
  short sVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  
  disableMaskableInterrupts();
  setInterruptMode(1);
  _DAT_ram_e7fe = 0x4087;
  bVar1 = func_0x0138();
  bVar1 = bVar1 >> 2 & 3;
  _DAT_ram_e7fe = 0x40a1;
  func_0x0024(*(byte *)(bVar1 - 0x33b) & 0xc | bVar1 | *(byte *)(bVar1 - 0x33f));
  DAT_ram_fd9a = 0xc3;
  _DAT_ram_fd9b = FUN_ram_402e;
  puVar4 = &DAT_ram_e000;
  puVar3 = &DAT_ram_e001;
  sVar2 = 0x7ff;
  DAT_ram_e000 = 0;
  do {
    *puVar3 = *puVar4;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
    sVar2 = sVar2 + -1;
  } while (sVar2 != 0);
  DAT_ram_e005 = 1;
  _DAT_ram_e7fe = 0x40c1;
  FUN_ram_44a0();
  DAT_ram_e005 = 0;
  _DAT_ram_e7fe = 0x40c8;
  func_0x013e();
  enableMaskableInterrupts();
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ram:40cb FUN_ram_40cb -- automatic, unverified */

void FUN_ram_40cb(void)

{
  FUN_ram_490c(&DAT_ram_44c6);
  return;
}



/* ram:40d3 FUN_ram_40d3 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_40d3(void)

{
  undefined2 uStack_2;
  
  DAT_ram_e003 = DAT_ram_e003 + '\x01';
  if (DAT_ram_e000 < 3) {
    register0x44 = (BADSPACEBASE *)&uStack_2;
    uStack_2 = 0x4262;
  }
                    /* WARNING: Subroutine does not return */
  *(undefined2 *)((short)register0x44 + -2) = 0x40ea;
  inline_dispatch(DAT_ram_e000);
}



/* ram:40fe state_40fe -- automatic, unverified */

void state_40fe(undefined2 param_1)

{
  byte in_F;
  byte bVar1;
  char cVar2;
  
  cVar2 = (char)((ushort)param_1 >> 8);
  if (cVar2 == '\x01') {
    bVar1 = in_F & 0xec;
    if ((DAT_ram_e003 & 1) == 0) {
      return;
    }
    FUN_ram_46f1(DAT_ram_e003 >> 1 | in_F << 7);
    if ((bVar1 >> 6 & 1) == 0) {
      return;
    }
    FUN_ram_440c(0x4518);
    DAT_ram_e004 = '\0';
  }
  else {
    if (cVar2 == '\x02') {
      DAT_ram_e004 = DAT_ram_e004 + -1;
      if (DAT_ram_e004 != '\0') {
        return;
      }
      FUN_ram_47ba();
      DAT_ram_e004 = 0;
      DAT_ram_e000 = DAT_ram_e000 + '\x01';
      DAT_ram_e001 = 0;
      return;
    }
    FUN_ram_44b4();
    FUN_ram_4386();
    FUN_ram_4595();
    FUN_ram_46cf();
  }
  DAT_ram_e001 = DAT_ram_e001 + '\x01';
  return;
}



/* ram:412e state_412e -- automatic, unverified */

void state_412e(void)

{
  DAT_ram_e004 = DAT_ram_e004 + -1;
  if (DAT_ram_e004 != '\0') {
    return;
  }
  DAT_ram_e004 = 0x20;
  DAT_ram_e000 = DAT_ram_e000 + '\x01';
  DAT_ram_e001 = 0;
  return;
}



/* ram:4135 state_4135 -- automatic, unverified */

char state_4135(undefined2 param_1)

{
  if ((char)((ushort)param_1 >> 8) != '\x01') {
    FUN_ram_44cd();
    FUN_ram_4386();
    FUN_ram_5a46();
    DAT_ram_e004 = 0x20;
    DAT_ram_e001 = DAT_ram_e001 + '\x01';
    return ' ';
  }
  FUN_ram_5a73();
  FUN_ram_56d2();
  if (DAT_ram_e064 != '\0') {
    return DAT_ram_e064;
  }
  DAT_ram_e062 = DAT_ram_e062 + 1 & 7;
  DAT_ram_e000 = 0;
  DAT_ram_e004 = 0x20;
  DAT_ram_e001 = 0;
  return '\0';
}



/* ram:416a state_416a -- automatic, unverified */

void state_416a(undefined2 param_1)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  byte *pbVar4;
  short sVar5;
  
  cVar3 = (char)((ushort)param_1 >> 8);
  if (cVar3 == '\x01') {
    DAT_ram_e004 = DAT_ram_e004 - 1;
    if (DAT_ram_e004 != 0) {
      pbVar4 = (byte *)0x4556;
      if ((DAT_ram_e004 & 4) != 0) {
        FUN_ram_4408();
        return;
      }
      bVar2 = 0xff;
      do {
        sVar5 = *(short *)pbVar4;
        pbVar4 = pbVar4 + 2;
        while( true ) {
          bVar1 = *pbVar4;
          pbVar4 = pbVar4 + 1;
          if (bVar1 == 0xff) {
            return;
          }
          if (bVar1 == 0xfe) break;
          func_0x004d(bVar1 & bVar2,sVar5);
          sVar5 = sVar5 + 1;
        }
      } while( true );
    }
  }
  else {
    if (cVar3 == '\x02') {
      FUN_ram_4290();
      FUN_ram_6f44();
      DAT_ram_e004 = 0x20;
      DAT_ram_e000 = DAT_ram_e000 + '\x01';
      DAT_ram_e001 = 0;
      return;
    }
    FUN_ram_490c(0xb7);
    DAT_ram_e004 = 0x40;
  }
  DAT_ram_e001 = DAT_ram_e001 + '\x01';
  return;
}



/* ram:419c state_419c -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x41cb) */

void state_419c(undefined2 param_1)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  
  if ((char)((ushort)param_1 >> 8) != '\x01') {
    FUN_ram_44cd();
    FUN_ram_4386();
    FUN_ram_565c();
    bVar1 = ((DAT_ram_e060 & 0xf) - 1 & 0x10) != 0;
    bVar2 = DAT_ram_e060 == 0;
    DAT_ram_e060 = BCDadjust(DAT_ram_e060 - 1,bVar2,bVar1);
    BCDadjustCarry(DAT_ram_e060,bVar2,bVar1);
    hasEvenParity(DAT_ram_e060);
    FUN_ram_431f();
    cVar3 = ',';
    FUN_ram_43f9(0x392c,0x4582);
    FUN_ram_4341(cVar3 + '\x01');
    DAT_ram_e004 = 0x78;
    DAT_ram_e001 = DAT_ram_e001 + '\x01';
    return;
  }
  if (DAT_ram_e073 == '\0') {
    if (DAT_ram_e02e != '\0') {
      return;
    }
    cVar3 = '\0';
  }
  else {
    DAT_ram_e004 = DAT_ram_e004 + -1;
    cVar3 = DAT_ram_e073;
    if (DAT_ram_e004 != '\0') {
      return;
    }
  }
  FUN_ram_53d3(cVar3);
  DAT_ram_e064 = 1;
  DAT_ram_e000 = DAT_ram_e000 + '\x01';
  DAT_ram_e001 = 0;
  return;
}



/* ram:41e1 state_41e1 -- automatic, unverified */

char state_41e1(void)

{
  FUN_ram_56d2();
  if (DAT_ram_e00d == '\0') {
    if (DAT_ram_e064 != '\0') {
      return DAT_ram_e064;
    }
    DAT_ram_e000 = DAT_ram_e000 + '\x01';
  }
  else {
    DAT_ram_e000 = '\b';
  }
  DAT_ram_e004 = 0x20;
  DAT_ram_e001 = 0;
  return '\0';
}



/* ram:41f4 state_41f4 -- automatic, unverified */

void state_41f4(void)

{
  if (DAT_ram_e060 == '\0') {
    FUN_ram_490c(0xba);
    FUN_ram_44cd();
    DAT_ram_e000 = DAT_ram_e000 + '\x01';
  }
  else {
    DAT_ram_e000 = '\x04';
  }
  DAT_ram_e004 = 0x20;
  DAT_ram_e001 = 0;
  return;
}



/* ram:420b state_420b -- automatic, unverified */

char state_420b(undefined2 param_1)

{
  char cVar1;
  
  if ((char)((ushort)param_1 >> 8) != '\x01') {
    FUN_ram_4386();
    FUN_ram_43f1(0x4588);
    cVar1 = FUN_ram_431f();
    DAT_ram_e001 = DAT_ram_e001 + '\x01';
    return cVar1;
  }
  if (DAT_ram_e012 != '\0') {
    return DAT_ram_e012;
  }
  DAT_ram_e002 = DAT_ram_e002 & 0xbf;
  DAT_ram_e000 = 0;
  DAT_ram_e004 = 0x20;
  DAT_ram_e001 = 0;
  return '\0';
}



/* ram:422b state_422b -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x424d) */
/* WARNING: Removing unreachable block (ram,0x4247) */

void state_422b(void)

{
  bool bVar1;
  undefined1 uVar2;
  
  DAT_ram_e00d = 0;
  DAT_ram_e073 = 0;
  if ((DAT_ram_e006 < 4) && (DAT_ram_e006 <= DAT_ram_e061)) {
    DAT_ram_e006 = DAT_ram_e061;
  }
  bVar1 = ((DAT_ram_e060 & 0xf) + 1 & 0x10) != 0;
  uVar2 = BCDadjust(DAT_ram_e060 + 1,0xfe < DAT_ram_e060,bVar1);
  BCDadjustCarry(uVar2,0xfe < DAT_ram_e060,bVar1);
  hasEvenParity(uVar2);
  DAT_ram_e060 = uVar2;
  bVar1 = ((DAT_ram_e061 & 0xf) + 1 & 0x10) != 0;
  uVar2 = BCDadjust(DAT_ram_e061 + 1,0xfe < DAT_ram_e061,bVar1);
  BCDadjustCarry(uVar2,0xfe < DAT_ram_e061,bVar1);
  hasEvenParity(uVar2);
  DAT_ram_e061 = uVar2;
  DAT_ram_e062 = DAT_ram_e062 + 1 & 7;
  if (DAT_ram_e062 == 0) {
    DAT_ram_e068 = 1;
    DAT_ram_e000 = DAT_ram_e000 + '\x01';
  }
  else {
    DAT_ram_e000 = '\x04';
  }
  DAT_ram_e004 = 0x20;
  DAT_ram_e001 = 0;
  return;
}



/* ram:425f thunk_FUN_ram_5b0b -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_ram_5b0b(undefined2 param_1)

{
  byte in_F;
  byte bVar1;
  ushort uVar2;
  char cVar3;
  char *pcVar4;
  undefined1 *puVar5;
  short sVar6;
  undefined1 *puVar7;
  
  cVar3 = (char)((ushort)param_1 >> 8);
  if (cVar3 == '\x01') {
    DAT_ram_e004 = DAT_ram_e004 + -1;
    if (DAT_ram_e004 != '\0') {
      return;
    }
    DAT_ram_e602 = 0x97;
    DAT_ram_e604 = 0x78;
    DAT_ram_e682 = 0;
    DAT_ram_e684 = 0x78;
    _DAT_ram_e676 = 0x70;
    FUN_ram_6112();
    FUN_ram_431f();
    FUN_ram_64a3();
  }
  else if (cVar3 == '\x02') {
    FUN_ram_60e1(&DAT_ram_e600);
    sVar6 = -0x1980;
    FUN_ram_60eb();
    pcVar4 = &DAT_ram_5c74;
    func_0x0053(0x3b30);
    uVar2 = CONCAT11(4,DAT_ram_0007);
    do {
      *(char *)(uVar2 & 0xff) = *pcVar4 + *(char *)(sVar6 + 2);
      *(char *)(uVar2 & 0xff) = pcVar4[1] + *(char *)(sVar6 + 4);
      cVar3 = (char)(uVar2 >> 8);
      *(char *)(uVar2 & 0xff) = ('\x04' - cVar3) * '\x04' + '\f';
      *(char *)(uVar2 & 0xff) = pcVar4[2];
      pcVar4 = pcVar4 + 3;
      cVar3 = cVar3 + -1;
      uVar2 = CONCAT11(cVar3,(char)uVar2);
    } while (cVar3 != '\0');
    if (*(byte *)(sVar6 + 2) < 0x48) {
      return;
    }
    _DAT_ram_e676 = 0xff60;
  }
  else if (cVar3 == '\x03') {
    puVar7 = &DAT_ram_e600;
    FUN_ram_60db();
    if (0x5a < (byte)puVar7[2]) {
      return;
    }
    DAT_ram_e690 = 'x';
  }
  else if (cVar3 == '\x04') {
    DAT_ram_e690 = DAT_ram_e690 + -1;
    if (DAT_ram_e690 != '\0') {
      return;
    }
    puVar5 = &DAT_ram_5c80;
    puVar7 = &DAT_ram_e6a0;
    sVar6 = 5;
    DAT_ram_e690 = '\0';
    do {
      *puVar7 = *puVar5;
      puVar7 = puVar7 + 1;
      puVar5 = puVar5 + 1;
      sVar6 = sVar6 + -1;
    } while (sVar6 != 0);
  }
  else if (cVar3 == '\x05') {
    bVar1 = in_F & 0xec | DAT_ram_e003 & 1;
    if ((bool)(DAT_ram_e003 & 1)) {
      return;
    }
    FUN_ram_5c06(DAT_ram_e003 >> 1 | in_F << 7);
    if (-1 < (char)bVar1) {
      return;
    }
    FUN_ram_43f1(0x5ca4);
  }
  else {
    if (cVar3 == '\x06') {
      DAT_ram_e690 = DAT_ram_e690 + -1;
      if (DAT_ram_e690 == '\0') {
        puVar5 = &DAT_ram_e080;
        puVar7 = &DAT_ram_e609;
        sVar6 = 7;
        DAT_ram_e690 = 0;
        do {
          *puVar7 = *puVar5;
          puVar7 = puVar7 + 1;
          puVar5 = puVar5 + 1;
          sVar6 = sVar6 + -1;
        } while (sVar6 != 0);
        DAT_ram_e000 = 4;
        DAT_ram_e004 = 0x20;
        DAT_ram_e001 = 0;
        return;
      }
      return;
    }
    if (DAT_ram_e012 != '\0') {
      return;
    }
    FUN_ram_4386(0);
    FUN_ram_490c(0xb4);
    puVar5 = &DAT_ram_e609;
    puVar7 = &DAT_ram_e080;
    sVar6 = 7;
    do {
      *puVar7 = *puVar5;
      puVar7 = puVar7 + 1;
      puVar5 = puVar5 + 1;
      sVar6 = sVar6 + -1;
    } while (sVar6 != 0);
    DAT_ram_e60c = 0;
    FUN_ram_45b5();
    FUN_ram_440c(0x5cb9);
    FUN_ram_43f1(0x5c85);
    DAT_ram_e004 = -0x4c;
  }
  DAT_ram_e001 = DAT_ram_e001 + '\x01';
  return;
}



/* ram:4290 FUN_ram_4290 -- automatic, unverified */

void FUN_ram_4290(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  puVar3 = &DAT_ram_e056;
  sVar1 = 0x6fa;
  puVar2 = &DAT_ram_e057;
  DAT_ram_e056 = 0;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  puVar3 = (undefined1 *)0x42a9;
  puVar2 = &DAT_ram_e060;
  sVar1 = 4;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}



/* ram:42ad FUN_ram_42ad -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x42df) */
/* WARNING: Removing unreachable block (ram,0x42c3) */
/* WARNING: Removing unreachable block (ram,0x42be) */
/* WARNING: Removing unreachable block (ram,0x42b9) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char FUN_ram_42ad(undefined2 param_1)

{
  bool bVar1;
  bool bVar2;
  byte bVar3;
  char cVar4;
  short sVar5;
  byte bVar6;
  byte *pbVar7;
  byte *pbVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  
  if (-1 < (char)(DAT_ram_e002 * '\x02')) {
    return DAT_ram_e002 * '\x02';
  }
  bVar6 = (byte)param_1;
  bVar1 = ((DAT_ram_e056 & 0xf) + (bVar6 & 0xf) & 0x10) != 0;
  bVar2 = CARRY1(DAT_ram_e056,bVar6);
  DAT_ram_e056 = BCDadjust(DAT_ram_e056 + bVar6,bVar2,bVar1);
  bVar6 = BCDadjustCarry(DAT_ram_e056,bVar2,bVar1);
  hasEvenParity(DAT_ram_e056);
  bVar3 = (byte)((ushort)param_1 >> 8);
  bVar6 = bVar6 & 1;
  bVar1 = ((DAT_ram_e057 & 0xf) + (bVar3 & 0xf) + bVar6 & 0x10) != 0;
  bVar2 = CARRY1(DAT_ram_e057,bVar3) || CARRY1(DAT_ram_e057 + bVar3,bVar6);
  DAT_ram_e057 = BCDadjust(DAT_ram_e057 + bVar3 + bVar6,bVar2,bVar1);
  bVar6 = BCDadjustCarry(DAT_ram_e057,bVar2,bVar1);
  hasEvenParity(DAT_ram_e057);
  bVar6 = bVar6 & 1;
  bVar1 = ((DAT_ram_e058 & 0xf) + bVar6 & 0x10) != 0;
  bVar3 = BCDadjust(DAT_ram_e058 + bVar6,CARRY1(DAT_ram_e058,bVar6),bVar1);
  bVar6 = BCDadjustCarry(bVar3,CARRY1(DAT_ram_e058,bVar6),bVar1);
  hasEvenParity(bVar3);
  DAT_ram_e058 = bVar3;
  if ((bVar6 & 1) == 0) {
    if (DAT_ram_e063 <= bVar3) {
      bVar1 = 0xef < DAT_ram_e063;
      DAT_ram_e063 = BCDadjust(DAT_ram_e063 + 0x10,bVar1,0);
      bVar6 = BCDadjustCarry(DAT_ram_e063,bVar1,0);
      hasEvenParity(DAT_ram_e063);
      if ((bVar6 & 1) != 0) {
        DAT_ram_e063 = 0xff;
      }
      state_4305();
    }
    pbVar7 = &DAT_ram_e058;
    cVar4 = '\x03';
    pbVar8 = &DAT_ram_e055;
    do {
      bVar3 = *pbVar8 - *pbVar7;
      if (*pbVar8 < *pbVar7) break;
      if (bVar3 != 0) goto LAB_ram_432b;
      pbVar7 = (byte *)CONCAT11((char)((ushort)pbVar7 >> 8),(char)pbVar7 + -1);
      pbVar8 = (byte *)CONCAT11((char)((ushort)pbVar8 >> 8),(char)pbVar8 + -1);
      cVar4 = cVar4 + -1;
    } while (cVar4 != '\0');
    sVar5 = 3;
    puVar9 = (undefined1 *)CONCAT11((char)((ushort)pbVar8 >> 8),0x55);
    puVar10 = (undefined1 *)CONCAT11((char)((ushort)pbVar7 >> 8),0x58);
    do {
      *puVar9 = *puVar10;
      puVar9 = puVar9 + -1;
      puVar10 = puVar10 + -1;
      sVar5 = sVar5 + -1;
    } while (sVar5 != 0);
  }
  else {
    _DAT_ram_e053 = 0x999999;
  }
LAB_ram_432b:
  FUN_ram_433a(bVar3,0x3aeb,&DAT_ram_e055);
  cVar4 = FUN_ram_4346(0x3ae2,&DAT_ram_e058);
  return cVar4;
}



/* ram:4305 state_4305 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x430e) */

void state_4305(void)

{
  bool bVar1;
  bool bVar2;
  
  if (DAT_ram_e060 != 0x99) {
    bVar1 = ((DAT_ram_e060 & 0xf) + 1 & 0x10) != 0;
    bVar2 = 0xfe < DAT_ram_e060;
    DAT_ram_e060 = BCDadjust(DAT_ram_e060 + 1,bVar2,bVar1);
    BCDadjustCarry(DAT_ram_e060,bVar2,bVar1);
    hasEvenParity(DAT_ram_e060);
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0x15);
  }
  return;
}



/* ram:4315 FUN_ram_4315 -- automatic, unverified */

void FUN_ram_4315(void)

{
  FUN_ram_4346(0x3af5,&DAT_ram_e060);
  return;
}



/* ram:431f FUN_ram_431f -- automatic, unverified */

void FUN_ram_431f(void)

{
  FUN_ram_43f1(0x4567);
  FUN_ram_4315();
  FUN_ram_433e();
  FUN_ram_433a(0x3aeb,&DAT_ram_e055);
  FUN_ram_4346(0x3ae2,&DAT_ram_e058);
  return;
}



/* ram:433a FUN_ram_433a -- automatic, unverified */

void FUN_ram_433a(void)

{
  FUN_ram_4346();
  return;
}



/* ram:433e FUN_ram_433e -- automatic, unverified */

void FUN_ram_433e(void)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  short sVar4;
  
  sVar4 = 0x3afc;
  pbVar3 = &DAT_ram_e061;
  cVar2 = '\x01';
  bVar1 = 0;
  do {
    if (*pbVar3 >> 4 != 0) {
      bVar1 = 0xff;
    }
    if (cVar2 == '\x01') {
      bVar1 = 0xff;
    }
    func_0x004d((*pbVar3 >> 4) + 0x10 & bVar1,sVar4);
    sVar4 = sVar4 + 1;
    if ((*pbVar3 & 0xf) != 0) {
      bVar1 = 0xff;
    }
    func_0x004d((*pbVar3 & 0xf) + 0x10 & bVar1,sVar4);
    pbVar3 = pbVar3 + -1;
    sVar4 = sVar4 + 1;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:4341 FUN_ram_4341 -- automatic, unverified */

void FUN_ram_4341(short param_1)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  
  pbVar3 = &DAT_ram_e061;
  cVar2 = '\x01';
  bVar1 = 0;
  do {
    if (*pbVar3 >> 4 != 0) {
      bVar1 = 0xff;
    }
    if (cVar2 == '\x01') {
      bVar1 = 0xff;
    }
    func_0x004d((*pbVar3 >> 4) + 0x10 & bVar1,param_1);
    param_1 = param_1 + 1;
    if ((*pbVar3 & 0xf) != 0) {
      bVar1 = 0xff;
    }
    func_0x004d((*pbVar3 & 0xf) + 0x10 & bVar1,param_1);
    pbVar3 = pbVar3 + -1;
    param_1 = param_1 + 1;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:4346 FUN_ram_4346 -- automatic, unverified */

void FUN_ram_4346(undefined2 param_1,short param_2,byte *param_3)

{
  byte bVar1;
  char cVar2;
  
  cVar2 = (char)((ushort)param_1 >> 8);
  bVar1 = 0;
  do {
    if (*param_3 >> 4 != 0) {
      bVar1 = 0xff;
    }
    if (cVar2 == '\x01') {
      bVar1 = 0xff;
    }
    func_0x004d((*param_3 >> 4) + 0x10 & bVar1,param_2);
    param_2 = param_2 + 1;
    if ((*param_3 & 0xf) != 0) {
      bVar1 = 0xff;
    }
    func_0x004d((*param_3 & 0xf) + 0x10 & bVar1,param_2);
    param_3 = param_3 + -1;
    param_2 = param_2 + 1;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:4372 FUN_ram_4372 -- automatic, unverified */

char FUN_ram_4372(byte param_1)

{
  return (param_1 >> 6) + 0x38;
}



/* ram:4386 FUN_ram_4386 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Possible PIC construction at 0x4396: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x4399) */
/* WARNING: Removing unreachable block (ram,0x43a4) */

void FUN_ram_4386(void)

{
  func_0x004d(0xd0,0x3b00);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:43b0 FUN_ram_43b0 -- automatic, unverified */

void FUN_ram_43b0(void)

{
  func_0x0053();
  return;
}



/* ram:43bc FUN_ram_43bc -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_43bc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:43c0 FUN_ram_43c0 -- automatic, unverified */

void FUN_ram_43c0(undefined2 param_1)

{
  char cVar1;
  
  cVar1 = '\x03';
  do {
    func_0x0056(param_1);
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:43d1 FUN_ram_43d1 -- automatic, unverified */

void FUN_ram_43d1(short param_1,undefined2 param_2)

{
  char cVar1;
  
  cVar1 = '\x03';
  do {
    FUN_ram_4412(param_1,param_2);
    param_1 = param_1 + 0x800;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:43e1 FUN_ram_43e1 -- automatic, unverified */

void FUN_ram_43e1(short param_1,undefined2 param_2)

{
  char cVar1;
  
  cVar1 = '\x03';
  do {
    FUN_ram_4474(param_1,param_2);
    param_1 = param_1 + 0x800;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:43f1 FUN_ram_43f1 -- automatic, unverified */

void FUN_ram_43f1(byte *param_1)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  
  bVar2 = 0xff;
  do {
    sVar3 = *(short *)param_1;
    param_1 = param_1 + 2;
    while( true ) {
      bVar1 = *param_1;
      param_1 = param_1 + 1;
      if (bVar1 == 0xff) {
        return;
      }
      if (bVar1 == 0xfe) break;
      func_0x004d(bVar1 & bVar2,sVar3);
      sVar3 = sVar3 + 1;
    }
  } while( true );
}



/* ram:43f9 FUN_ram_43f9 -- automatic, unverified */

void FUN_ram_43f9(byte param_1,short param_2,byte *param_3)

{
  byte bVar1;
  byte *pbVar2;
  
  while( true ) {
    bVar1 = *param_3;
    pbVar2 = param_3 + 1;
    if (bVar1 == 0xff) break;
    if (bVar1 == 0xfe) {
      param_2 = *(short *)pbVar2;
      param_3 = param_3 + 3;
    }
    else {
      func_0x004d(bVar1 & param_1,param_2);
      param_2 = param_2 + 1;
      param_3 = pbVar2;
    }
  }
  return;
}



/* ram:4408 FUN_ram_4408 -- automatic, unverified */

void FUN_ram_4408(byte *param_1)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  
  bVar2 = 0;
  do {
    sVar3 = *(short *)param_1;
    param_1 = param_1 + 2;
    while( true ) {
      bVar1 = *param_1;
      param_1 = param_1 + 1;
      if (bVar1 == 0xff) {
        return;
      }
      if (bVar1 == 0xfe) break;
      func_0x004d(bVar1 & bVar2,sVar3);
      sVar3 = sVar3 + 1;
    }
  } while( true );
}



/* ram:440c FUN_ram_440c -- automatic, unverified */

void FUN_ram_440c(byte *param_1)

{
  undefined2 uVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort unaff_BC_;
  
  do {
    uVar1 = *(undefined2 *)param_1;
    param_1 = param_1 + 2;
    FUN_ram_43b0(uVar1);
    while( true ) {
      while( true ) {
        bVar2 = *param_1;
        if (bVar2 == 0) {
          return;
        }
        param_1 = param_1 + 1;
        if ((bVar2 & 0x7f) != bVar2) break;
        uVar3 = FUN_ram_443b();
        do {
          *(undefined1 *)(unaff_BC_ & 0xff) = uVar3;
          bVar2 = bVar2 - 1;
        } while (bVar2 != 0);
      }
      if ((bVar2 & 0x7f) == 0) break;
      bVar2 = bVar2 & 0x7f;
      do {
        uVar3 = FUN_ram_443b();
        *(undefined1 *)(unaff_BC_ & 0xff) = uVar3;
        bVar2 = bVar2 - 1;
      } while (bVar2 != 0);
    }
  } while( true );
}



/* ram:4412 FUN_ram_4412 -- automatic, unverified */

void FUN_ram_4412(undefined2 param_1,byte *param_2)

{
  byte bVar1;
  undefined1 uVar2;
  byte *pbVar3;
  ushort unaff_BC_;
  
  do {
    FUN_ram_43b0(param_1);
    while( true ) {
      while( true ) {
        bVar1 = *param_2;
        if (bVar1 == 0) {
          return;
        }
        pbVar3 = param_2 + 1;
        if ((bVar1 & 0x7f) != bVar1) break;
        uVar2 = FUN_ram_443b();
        do {
          *(undefined1 *)(unaff_BC_ & 0xff) = uVar2;
          bVar1 = bVar1 - 1;
          param_2 = pbVar3;
        } while (bVar1 != 0);
      }
      if ((bVar1 & 0x7f) == 0) break;
      bVar1 = bVar1 & 0x7f;
      do {
        uVar2 = FUN_ram_443b();
        *(undefined1 *)(unaff_BC_ & 0xff) = uVar2;
        bVar1 = bVar1 - 1;
        param_2 = pbVar3;
      } while (bVar1 != 0);
    }
    param_1 = *(undefined2 *)pbVar3;
    param_2 = param_2 + 3;
  } while( true );
}



/* ram:443b FUN_ram_443b -- automatic, unverified */

byte FUN_ram_443b(ushort param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  char cVar5;
  
  bVar3 = *param_2;
  if (DAT_ram_e661 == 0) {
    if ((param_1 & 1) == 0) {
      return bVar3;
    }
    cVar5 = '\b';
    bVar2 = 0;
    bVar4 = bVar3;
    do {
      bVar1 = bVar4 & 1;
      bVar4 = bVar4 >> 1 | bVar2 << 7;
      bVar2 = bVar3 >> 7;
      bVar3 = bVar3 << 1 | bVar1;
      cVar5 = cVar5 + -1;
    } while (cVar5 != '\0');
    return bVar3;
  }
  if ((((bVar3 != 0xe1) && (bVar3 != 0xec)) && (bVar3 != 0xe8)) &&
     ((bVar3 != 0xe6 && (bVar3 != 0xe5)))) {
    return bVar3;
  }
  if ((DAT_ram_e661 & 2) == 0) {
    return bVar3 + 0xb0;
  }
  return bVar3 + 0x60;
}



/* ram:4469 FUN_ram_4469 -- automatic, unverified */

void FUN_ram_4469(void)

{
  char cVar1;
  
  cVar1 = '\b';
  do {
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:4474 FUN_ram_4474 -- automatic, unverified */

void FUN_ram_4474(undefined2 param_1,byte *param_2)

{
  byte bVar1;
  undefined1 uVar2;
  byte *pbVar3;
  ushort unaff_BC_;
  
  do {
    FUN_ram_43b0(param_1);
    while( true ) {
      while( true ) {
        bVar1 = *param_2;
        if (bVar1 == 0) {
          return;
        }
        pbVar3 = param_2 + 1;
        if ((bVar1 & 0x7f) != bVar1) break;
        uVar2 = FUN_ram_443b();
        do {
          *(undefined1 *)(unaff_BC_ & 0xff) = uVar2;
          bVar1 = bVar1 - 1;
          param_2 = pbVar3;
        } while (bVar1 != 0);
      }
      if ((bVar1 & 0x7f) == 0) break;
      bVar1 = bVar1 & 0x7f;
      do {
        uVar2 = FUN_ram_443b();
        *(undefined1 *)(unaff_BC_ & 0xff) = uVar2;
        bVar1 = bVar1 - 1;
        param_2 = pbVar3;
      } while (bVar1 != 0);
    }
    param_1 = *(undefined2 *)pbVar3;
    param_2 = param_2 + 3;
  } while( true );
}



/* ram:4478 FUN_ram_4478 -- automatic, unverified */

void FUN_ram_4478(char param_1)

{
  do {
    FUN_ram_4484();
    FUN_ram_4060(0x20);
    param_1 = param_1 + -1;
  } while (param_1 != '\0');
  return;
}



/* ram:4484 FUN_ram_4484 -- automatic, unverified */

void FUN_ram_4484(short param_1,ushort param_2)

{
  char cVar1;
  char cVar2;
  ushort uVar3;
  
  uVar3 = param_2;
  do {
    cVar1 = '\x10';
    do {
      cVar2 = (char)param_2;
      func_0x004a(param_1,uVar3);
      FUN_ram_4469();
      func_0x004d();
      param_2 = (ushort)(byte)(cVar2 + 1);
      param_1 = param_1 + 1;
      cVar1 = cVar1 + -1;
    } while (cVar1 != '\0');
    param_2 = (ushort)(byte)(cVar2 - 0x1fU);
  } while ((cVar2 - 0x1fU & 0x10) == 0);
  return;
}



/* ram:44a0 FUN_ram_44a0 -- automatic, unverified */

void FUN_ram_44a0(void)

{
  char cVar1;
  
  FUN_ram_49b7(0xb8);
  FUN_ram_40cb(0x3d);
  func_0x0056(0,0x4000,0);
  cVar1 = '\b';
  do {
    func_0x0047();
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:44b4 FUN_ram_44b4 -- automatic, unverified */

void FUN_ram_44b4(void)

{
  char cVar1;
  
  cVar1 = '\b';
  do {
    func_0x0047();
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:44cd FUN_ram_44cd -- automatic, unverified */

void FUN_ram_44cd(void)

{
  FUN_ram_4052(7);
  return;
}



/* ram:44d2 FUN_ram_44d2 -- automatic, unverified */

void FUN_ram_44d2(void)

{
  byte bVar1;
  byte bVar2;
  
  bVar2 = FUN_ram_44df();
  bVar1 = DAT_ram_e009;
  DAT_ram_e009 = bVar2;
  DAT_ram_e008 = (bVar2 ^ bVar1) & bVar2;
  return;
}



/* ram:44df FUN_ram_44df -- automatic, unverified */

byte FUN_ram_44df(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  
  func_0x0093(0xf,0x8f);
  disableMaskableInterrupts();
  bVar2 = func_0x0096(0xe);
  enableMaskableInterrupts();
  bVar3 = func_0x0141(7);
  bVar3 = (byte)~bVar3 >> 1 & 0x20;
  bVar4 = func_0x0141(8,bVar3);
  bVar1 = (byte)~bVar4 >> 1;
  return (bVar1 & 0x30) >> 4 | (byte)(bVar1 & 0xc0 | ~bVar4 << 7) >> 3 | (bVar1 & 8) >> 1 | bVar3 |
         ~bVar2 & 0x3f;
}



/* ram:4595 FUN_ram_4595 -- automatic, unverified */

void FUN_ram_4595(void)

{
  char cVar1;
  char cVar2;
  short sVar3;
  short sVar4;
  
  cVar1 = FUN_ram_43c0(0,0x2000);
  sVar4 = 0;
  sVar3 = 8;
  cVar2 = '\x10';
  do {
    cVar1 = FUN_ram_43c0(cVar1,8,sVar4);
    sVar4 = sVar4 + sVar3;
    cVar1 = cVar1 + '\x01';
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  FUN_ram_43d1(0x2080,0x45c9);
  FUN_ram_43c0(0xf0,0x118,0x80);
  return;
}



/* ram:45b5 FUN_ram_45b5 -- automatic, unverified */

void FUN_ram_45b5(void)

{
  FUN_ram_43d1(0x2080,0x45c9);
  FUN_ram_43c0(0xf0,0x118,0x80);
  return;
}



/* ram:46cf FUN_ram_46cf -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_46cf(void)

{
  DAT_ram_e00a = 0xe;
  _DAT_ram_e00e = 0x3aaa;
  FUN_ram_4025();
  return;
}



/* ram:46dd FUN_ram_46dd -- automatic, unverified */

void FUN_ram_46dd(void)

{
  FUN_ram_43d1(0x6200,0x4723);
  FUN_ram_43c0(0xf0,0xd8,0x200);
  return;
}



/* ram:46f1 FUN_ram_46f1 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_46f1(void)

{
  _DAT_ram_e00e = _DAT_ram_e00e + -0x20;
  FUN_ram_4715(0x40);
  FUN_ram_4715(0xb0c);
  FUN_ram_4715();
  func_0x0056(0);
  DAT_ram_e00a = DAT_ram_e00a + -1;
  return;
}



/* ram:4715 FUN_ram_4715 -- automatic, unverified */

void FUN_ram_4715(char param_1,undefined2 param_2,short param_3)

{
  char cVar1;
  short sVar2;
  
  cVar1 = (char)((ushort)param_2 >> 8);
  sVar2 = param_3;
  do {
    param_1 = func_0x004d(param_1,param_3,sVar2);
    param_3 = param_3 + 1;
    param_1 = param_1 + '\x01';
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:47ba FUN_ram_47ba -- automatic, unverified */

void FUN_ram_47ba(void)

{
  char cVar1;
  char cVar2;
  char cVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  
  FUN_ram_44cd();
  FUN_ram_4386();
  FUN_ram_45b5();
  FUN_ram_43d1(0x2300,0x47f6);
  FUN_ram_43d1(0x300,0x48fa);
  sVar5 = 0x38cb;
  sVar4 = 0x20;
  cVar1 = '`';
  cVar2 = '\x03';
  do {
    cVar3 = '\v';
    sVar6 = sVar5;
    do {
      cVar1 = func_0x004d(cVar1,sVar6);
      sVar6 = sVar6 + 1;
      cVar1 = cVar1 + '\x01';
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    sVar5 = sVar5 + sVar4;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  FUN_ram_43f1(0x4529);
  return;
}



/* ram:4901 FUN_ram_4901 -- automatic, unverified */

byte FUN_ram_4901(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  short in_AF;
  byte bVar1;
  byte bVar3;
  char cVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte bVar2;
  
  disableMaskableInterrupts();
  bVar1 = (byte)((ushort)in_AF >> 8);
  bVar2 = bVar1;
  if ((DAT_ram_e002 & 0x40) == 0) goto LAB_ram_4976;
  bVar3 = bVar1 & 0x3f;
  cVar4 = '\x02';
  pbVar6 = &DAT_ram_e02e;
  if (bVar3 < 0x1d) {
    cVar4 = '\x01';
    if ((0x17 < bVar3) && (pbVar6 = &DAT_ram_e012, 0x19 < bVar3)) {
      pbVar6 = &DAT_ram_e020;
    }
    bVar3 = bVar1;
    if (-1 < in_AF) {
      if (DAT_ram_e600 == '\x02') goto LAB_ram_4976;
      goto LAB_ram_4946;
    }
  }
  else {
    pbVar6 = &DAT_ram_e012;
    if (0x24 < bVar3) {
      pbVar6 = &DAT_ram_e012;
      cVar4 = '\x03';
    }
LAB_ram_4946:
    bVar3 = bVar1 & 0x3f;
    if ((bVar1 & 0x3f) < (*pbVar6 & 0x3f)) goto LAB_ram_4976;
  }
  pbVar5 = &DAT_ram_4b36;
  FUN_ram_4060((bVar3 & 0x3f) * '\x02',param_1,param_3,param_2);
  pbVar6 = pbVar6 + -2;
  do {
    *pbVar6 = 1;
    pbVar6[2] = bVar1;
    pbVar6[3] = *pbVar5;
    pbVar6 = pbVar6 + 4;
    pbVar5 = pbVar5 + 1;
    *pbVar6 = *pbVar5;
    FUN_ram_405b(5);
    *pbVar6 = 0;
    FUN_ram_405b(5);
    pbVar5 = pbVar5 + 1;
    cVar4 = cVar4 + -1;
  } while (cVar4 != '\0');
LAB_ram_4976:
  enableMaskableInterrupts();
  return bVar2;
}



/* ram:490c FUN_ram_490c -- automatic, unverified */

byte FUN_ram_490c(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  short in_AF;
  byte bVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  byte *pbVar5;
  byte *pbVar6;
  
  disableMaskableInterrupts();
  bVar1 = (byte)((ushort)in_AF >> 8);
  bVar2 = bVar1 & 0x3f;
  cVar4 = '\x02';
  pbVar6 = &DAT_ram_e02e;
  if (bVar2 < 0x1d) {
    cVar4 = '\x01';
    if ((0x17 < bVar2) && (pbVar6 = &DAT_ram_e012, 0x19 < bVar2)) {
      pbVar6 = &DAT_ram_e020;
    }
    bVar2 = bVar1;
    if (-1 < in_AF) {
      if (DAT_ram_e600 == '\x02') goto LAB_ram_4973;
      goto LAB_ram_4946;
    }
  }
  else {
    pbVar6 = &DAT_ram_e012;
    if (0x24 < bVar2) {
      pbVar6 = &DAT_ram_e012;
      cVar4 = '\x03';
    }
LAB_ram_4946:
    bVar2 = bVar1 & 0x3f;
    if ((bVar1 & 0x3f) < (*pbVar6 & 0x3f)) goto LAB_ram_4973;
  }
  pbVar5 = &DAT_ram_4b36;
  bVar3 = bVar1;
  FUN_ram_4060((bVar2 & 0x3f) * '\x02',param_1,param_3,param_2);
  pbVar6 = pbVar6 + -2;
  do {
    *pbVar6 = 1;
    pbVar6[2] = bVar3;
    pbVar6[3] = *pbVar5;
    pbVar6 = pbVar6 + 4;
    pbVar5 = pbVar5 + 1;
    *pbVar6 = *pbVar5;
    FUN_ram_405b(5);
    *pbVar6 = 0;
    FUN_ram_405b(5);
    pbVar5 = pbVar5 + 1;
    cVar4 = cVar4 + -1;
  } while (cVar4 != '\0');
LAB_ram_4973:
  enableMaskableInterrupts();
  return bVar1;
}



/* ram:49a2 FUN_ram_49a2 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_49a2(byte param_1,undefined2 param_2)

{
  byte bVar1;
  
  if (param_1 != 1) {
    param_1 = param_1 - 1;
  }
  bVar1 = ((param_1 << 1 | param_1 >> 7) << 1 | (param_1 & 0x7f) >> 6) << 1 |
          (param_1 << 1 & 0x7f) >> 6;
  if ((char)((ushort)param_2 >> 8) != '\x01') {
    FUN_ram_49b7(~bVar1 & DAT_ram_e03a);
    return;
  }
  DAT_ram_e03a = bVar1 | DAT_ram_e03a;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:49b7 FUN_ram_49b7 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_49b7(undefined1 param_1)

{
  DAT_ram_e03a = param_1;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:49c0 audio_tick -- automatic, unverified */

void audio_tick(undefined2 param_1,undefined2 param_2)

{
  char cVar1;
  undefined1 *puVar2;
  
  FUN_ram_49b7(DAT_ram_e03a);
  puVar2 = &DAT_ram_e010;
  cVar1 = '\x03';
  do {
    if (puVar2[2] == '\0') {
      FUN_ram_4a59();
    }
    else {
      FUN_ram_49e9(puVar2[2],param_1,param_2);
    }
    puVar2 = puVar2 + 0xe;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:49e9 FUN_ram_49e9 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Possible PIC construction at 0x4a2c: Changing call to branch */
/* WARNING: Possible PIC construction at 0x4b1b: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x4a2f) */
/* WARNING: Removing unreachable block (ram,0x4b1e) */

byte FUN_ram_49e9(byte param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  undefined2 uVar3;
  byte *pbVar4;
  byte *pbVar5;
  
  if ((param_1 & 0x40) == 0) {
    FUN_ram_49a2();
  }
  while( true ) {
    if ((char)param_2[2] < '\0') {
      bVar1 = *param_2;
      *param_2 = bVar1 - 1;
      if ((byte)(bVar1 - 1) != 0) {
        param_2[8] = param_2[8] - 1;
        bVar1 = param_2[8];
        if (bVar1 == *param_2) {
          if (param_2[0xd] < bVar1) {
            return bVar1;
          }
        }
        else {
          param_2[8] = param_2[8] - 1;
        }
        bVar1 = param_2[7] - 1;
        if (-1 < (char)bVar1) {
          param_2[7] = bVar1;
          halt_baddata();
        }
        return bVar1;
      }
    }
    else {
      bVar1 = *param_2;
      *param_2 = bVar1 - 1;
      if ((byte)(bVar1 - 1) != 0) {
        return param_2[2];
      }
    }
    pbVar4 = *(byte **)(param_2 + 3);
    bVar1 = *pbVar4;
    if (bVar1 != 0xfe) break;
    bVar1 = param_2[9] + 1;
    if (bVar1 == pbVar4[1]) {
      param_2[9] = 0;
      FUN_ram_4b24(pbVar4 + 3);
    }
    else {
      if (-1 < (char)(bVar1 - pbVar4[1])) {
        bVar1 = param_2[9];
      }
      param_2[9] = bVar1;
      param_2[3] = pbVar4[2];
      param_2[4] = pbVar4[3];
    }
    *param_2 = *param_2 + 1;
  }
  if (bVar1 < 0xfe) {
    if ((param_2[2] & 0x80) == 0) {
      if ((bVar1 & 0xf0) == 0x20) {
        param_2[1] = *pbVar4 & 0xf;
        pbVar4 = pbVar4 + 1;
      }
      if ((*pbVar4 & 0xf0) != 0x10) {
        pbVar5 = pbVar4 + 1;
        uVar3 = CONCAT11(*pbVar4 & 0xf0 ^ *pbVar4,*pbVar5);
        FUN_ram_4b24(uVar3);
        FUN_ram_4b12(uVar3,pbVar5);
        *param_2 = param_2[1];
        param_2[8] = param_2[0xc] + param_2[1];
      }
    }
    else {
      while( true ) {
        bVar1 = *pbVar4;
        if ((*pbVar4 & 0xf0) == 0xd0) {
          param_2[10] = bVar1 & 0xf;
          pbVar4 = pbVar4 + 1;
          bVar1 = *pbVar4;
        }
        if (0xef < bVar1) {
          param_2[6] = bVar1 & 0xf;
          param_2[0xd] = pbVar4[1] & 0xf;
          param_2[0xc] = pbVar4[1] >> 4;
          pbVar4 = pbVar4 + 2;
          bVar1 = *pbVar4;
        }
        if (bVar1 < 0xe0) goto LAB_ram_4ad6;
        if ((bVar1 & 8) == 0) break;
        param_2[0xb] = bVar1 & 0xf;
        pbVar4 = pbVar4 + 1;
      }
      param_2[5] = bVar1 & 0xf;
      pbVar4 = pbVar4 + 1;
      bVar1 = *pbVar4;
LAB_ram_4ad6:
      bVar2 = param_2[10];
      for (bVar1 = bVar1 & 0xf; bVar1 != 0; bVar1 = bVar1 - 1) {
        bVar2 = bVar2 + param_2[10];
      }
      param_2[1] = bVar2;
      bVar2 = FUN_ram_4b24(*pbVar4);
      bVar2 = bVar2 >> 4;
      bVar1 = 0;
      if (bVar2 != 0xc) {
        bVar1 = param_2[6];
      }
      param_2[7] = bVar1;
      FUN_ram_4a49();
      FUN_ram_405b(bVar2);
      for (bVar1 = param_2[5]; bVar1 != 0; bVar1 = bVar1 - 1) {
      }
    }
  }
  else {
    FUN_ram_49a2();
    param_2[2] = 0;
    param_2[0xb] = 0;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:4a49 FUN_ram_4a49 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_4a49(char *param_1)

{
  *param_1 = param_1[1];
  param_1[8] = param_1[0xc] + param_1[1];
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:4a59 FUN_ram_4a59 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void FUN_ram_4a59(short param_1)

{
  FUN_ram_49a2();
  *(undefined1 *)(param_1 + 2) = 0;
  *(undefined1 *)(param_1 + 0xb) = 0;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:4b12 FUN_ram_4b12 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Possible PIC construction at 0x4b1b: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x4b1e) */

void FUN_ram_4b12(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:4b24 FUN_ram_4b24 -- automatic, unverified */

void FUN_ram_4b24(short param_1,short param_2)

{
  *(short *)(param_2 + 3) = param_1 + 1;
  return;
}



/* ram:4ecd state_4ecd -- automatic, unverified */
/* Exception while decompiling ram:4ecd: Decompiler process died
 */

/* ram:53d3 FUN_ram_53d3 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_53d3(void)

{
  undefined2 uVar1;
  char cVar2;
  byte bVar3;
  short sVar4;
  char cVar6;
  ushort uVar5;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  byte *pbVar10;
  char *pcVar11;
  undefined1 uVar13;
  short sVar12;
  
  if (DAT_ram_e092 != '\t') {
    DAT_ram_e3b0 = '\0';
  }
  if (DAT_ram_e3b0 == '\0') {
    FUN_ram_568c();
  }
  else {
    FUN_ram_569d();
  }
  puVar8 = &DAT_ram_e609;
  puVar7 = &DAT_ram_e080;
  sVar4 = 7;
  do {
    *puVar7 = *puVar8;
    puVar7 = puVar7 + 1;
    puVar8 = puVar8 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  puVar8 = &DAT_ram_e600;
  puVar7 = (undefined1 *)0xe601;
  sVar4 = 0x14f;
  DAT_ram_e600 = 0;
  do {
    *puVar7 = *puVar8;
    puVar7 = puVar7 + 1;
    puVar8 = puVar8 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  puVar8 = &DAT_ram_e600;
  puVar7 = &DAT_ram_e4f0;
  sVar4 = 0x80;
  do {
    *puVar7 = *puVar8;
    puVar7 = puVar7 + 1;
    puVar8 = puVar8 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  if (DAT_ram_e062 == '\x05') {
    DAT_ram_e3f0 = 0x80;
  }
  if (DAT_ram_e062 == '\x06') {
    DAT_ram_e3d0 = 0;
  }
  DAT_ram_e590 = 0;
  DAT_ram_e5b0 = 0;
  DAT_ram_e5d0 = 0;
  if (DAT_ram_e073 == '\0') {
    DAT_ram_e092 = DAT_ram_e073;
    DAT_ram_e09f = DAT_ram_e073;
    DAT_ram_e073 = '\x01';
    sVar4 = -0x6633;
    FUN_ram_4071(DAT_ram_e062 * '\x02');
    _DAT_ram_e095 = *(short *)(sVar4 + 1);
    DAT_ram_e065 = DAT_ram_e062;
    puVar9 = &DAT_ram_e080;
    puVar8 = &DAT_ram_e609;
    sVar4 = 7;
    _DAT_ram_e093 = puVar7;
    do {
      *puVar8 = *puVar9;
      puVar8 = puVar8 + 1;
      puVar9 = puVar9 + 1;
      sVar4 = sVar4 + -1;
    } while (sVar4 != 0);
  }
  DAT_ram_e602 = 0x87;
  DAT_ram_e604 = 0x78;
  pbVar10 = &DAT_ram_e620;
  bVar3 = 0;
  cVar6 = '\x04';
  do {
    *pbVar10 = bVar3 | 0x80;
    pbVar10 = pbVar10 + 0x10;
    bVar3 = bVar3 + 1;
    cVar6 = cVar6 + -1;
  } while (cVar6 != '\0');
  puVar8 = &DAT_ram_e0a0;
  puVar7 = (undefined1 *)0xe0a1;
  sVar4 = 0x220;
  DAT_ram_e0a0 = 0;
  do {
    *puVar7 = *puVar8;
    puVar7 = puVar7 + 1;
    puVar8 = puVar8 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  DAT_ram_e09e = 0;
  FUN_ram_4071(DAT_ram_e062,0x704b);
  pbVar10 = &DAT_ram_e09f;
  DAT_ram_e09f = DAT_ram_e09f + -1;
  do {
    bVar3 = (byte)sVar4;
    *pbVar10 = *pbVar10 - 1;
    if (0xfd < *pbVar10) {
      *pbVar10 = 0;
      goto LAB_ram_54b5;
    }
    puVar8 = puVar7;
    FUN_ram_4060(*pbVar10 * '\x02');
    bVar3 = (byte)sVar4;
  } while ((puVar8[1] & 0xf) != 0);
  *pbVar10 = *pbVar10 + 1;
LAB_ram_54b5:
  FUN_ram_7613();
  FUN_ram_66a3();
  FUN_ram_440c(0xa71e);
  puVar7 = (undefined1 *)0xee20;
  pcVar11 = (char *)0x5615;
  cVar6 = '\x02';
  do {
    uVar1 = *(undefined2 *)pcVar11;
    pcVar11 = pcVar11 + 2;
    puVar8 = puVar7;
    FUN_ram_5638(puVar7 + 0x10,uVar1);
    puVar9 = puVar7 + 0x20;
    sVar4 = 0x10;
    do {
      *puVar7 = *puVar9;
      puVar7 = puVar7 + 1;
      puVar9 = puVar9 + 1;
      sVar4 = sVar4 + -1;
    } while (sVar4 != 0);
    puVar7 = puVar8 + 0x20;
    cVar6 = cVar6 + -1;
  } while (cVar6 != '\0');
  if (DAT_ram_e3b0 != '\0') {
    FUN_ram_56b2();
    *pcVar11 = *pcVar11 + -1;
  }
  sVar4 = 0x563c;
  FUN_ram_4071(DAT_ram_e062 * '\x02');
  FUN_ram_43d1(0x2280);
  FUN_ram_43e1(0x2580);
  uVar13 = 1;
  if ((DAT_ram_e062 == '\x03') || (uVar13 = 2, DAT_ram_e062 == '\a')) {
    DAT_ram_e661 = uVar13;
  }
  FUN_ram_43d1(0x280,*(undefined2 *)(sVar4 + 1));
  FUN_ram_43d1(0x580);
  DAT_ram_e661 = 0;
  if (DAT_ram_e062 == '\a') {
    FUN_ram_43d1(0x2470,0xbe41);
    FUN_ram_43e1(0x2770);
    FUN_ram_43d1(0x470,0xbeb7);
    FUN_ram_43d1(0x770);
  }
  DAT_ram_e090 = 0;
  uVar5 = (ushort)bVar3;
  DAT_ram_e091 = DAT_ram_e092 * '\x18';
  _DAT_ram_e088 = _DAT_ram_e095 - (ushort)(byte)(DAT_ram_e092 * '\x06');
  puVar7 = _DAT_ram_e093;
  DAT_ram_e09a = DAT_ram_e090;
  FUN_ram_4079(DAT_ram_e092);
  bVar3 = 0;
  _DAT_ram_e097 = puVar7;
  _DAT_ram_e09c = puVar7;
  do {
    FUN_ram_6502();
    cVar6 = (char)uVar5;
    bVar3 = bVar3 + 1;
  } while (bVar3 < 7);
  if (DAT_ram_e3b0 != '\0') {
    FUN_ram_8d87();
  }
  FUN_ram_55ed();
  sVar12 = 0x6a98;
  FUN_ram_4072(DAT_ram_e062 * '\f');
  sVar12 = sVar12 + 1;
  sVar4 = sVar12;
  FUN_ram_5607(DAT_ram_e092);
  _DAT_ram_e570 = sVar4;
  if ((-1 < (char)(DAT_ram_e092 + -1)) && ((char)(DAT_ram_e092 + -1) != '\b')) {
    cVar2 = FUN_ram_5607();
    cVar2 = cVar2 - cVar6;
    puVar7 = &DAT_ram_e4f0;
    do {
      FUN_ram_6a1e();
      puVar7[2] = (DAT_ram_e091 - *(char *)(sVar12 + -2)) * '\b';
      puVar7 = puVar7 + 0x10;
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
  }
  FUN_ram_64a3();
  return;
}



/* ram:55ed FUN_ram_55ed -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_55ed(void)

{
  char cVar1;
  byte in_F;
  byte bVar2;
  char cVar3;
  
  bVar2 = *(byte *)((short)_DAT_ram_e088 + -1);
  _DAT_ram_e088 = (byte *)((short)_DAT_ram_e088 + -1);
  cVar3 = '\b';
  do {
    cVar1 = (char)bVar2 >> 7;
    bVar2 = bVar2 << 1 | in_F & 1;
    if (-cVar1 != 0) {
      _DAT_ram_e097 = _DAT_ram_e097 + -1;
    }
    cVar3 = cVar3 + -1;
    in_F = -cVar1;
  } while (cVar3 != '\0');
  _DAT_ram_e09c = _DAT_ram_e097;
  return;
}



/* ram:5607 FUN_ram_5607 -- automatic, unverified */

void FUN_ram_5607(void)

{
  FUN_ram_405b();
  return;
}



/* ram:561b FUN_ram_561b -- automatic, unverified */

void FUN_ram_561b(undefined1 *param_1,byte *param_2)

{
  byte bVar1;
  undefined1 uVar2;
  byte bVar3;
  
  while( true ) {
    bVar1 = *param_2;
    param_2 = param_2 + 1;
    if (bVar1 == 0) break;
    bVar3 = bVar1 & 0x7f;
    if (bVar3 == bVar1) {
      uVar2 = FUN_ram_443b();
      do {
        *param_1 = uVar2;
        param_1 = param_1 + 1;
        bVar3 = bVar3 - 1;
      } while (bVar3 != 0);
    }
    else {
      do {
        uVar2 = FUN_ram_443b();
        *param_1 = uVar2;
        param_1 = param_1 + 1;
        bVar3 = bVar3 - 1;
      } while (bVar3 != 0);
    }
  }
  return;
}



/* ram:5638 FUN_ram_5638 -- automatic, unverified */

void FUN_ram_5638(void)

{
  FUN_ram_561b(1);
  return;
}



/* ram:565c FUN_ram_565c -- automatic, unverified */

void FUN_ram_565c(void)

{
  FUN_ram_43d1(0x2138,0xb17b);
  FUN_ram_43e1(0x2438);
  FUN_ram_43d1(&SUB_ram_0138,0xb285);
  FUN_ram_43d1(0x438);
  FUN_ram_43d1(0x2500,0xb2fb);
  FUN_ram_43d1(0x500,0xb314);
  return;
}



/* ram:568c FUN_ram_568c -- automatic, unverified */

void FUN_ram_568c(void)

{
  undefined1 *puVar1;
  
  DAT_ram_e667 = 0;
  puVar1 = &DAT_ram_56a2;
  FUN_ram_405b(DAT_ram_e062);
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(*puVar1);
}



/* ram:5690 FUN_ram_5690 -- automatic, unverified */

void FUN_ram_5690(void)

{
  undefined1 *puVar1;
  
  puVar1 = &DAT_ram_56a2;
  FUN_ram_405b(DAT_ram_e062);
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(*puVar1);
}



/* ram:569d FUN_ram_569d -- automatic, unverified */

void FUN_ram_569d(void)

{
  undefined1 *puVar1;
  
  puVar1 = (undefined1 *)0x56aa;
  FUN_ram_405b(DAT_ram_e062);
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(*puVar1);
}



/* ram:56b2 FUN_ram_56b2 -- automatic, unverified */

void FUN_ram_56b2(void)

{
                    /* WARNING: Subroutine does not return */
  inline_dispatch(DAT_ram_e065);
}



/* ram:56d2 FUN_ram_56d2 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x5889) */
/* WARNING: Removing unreachable block (ram,0x587f) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_ram_56d2(void)

{
  bool bVar1;
  undefined1 *puVar2;
  byte bVar3;
  undefined1 uVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  char cVar8;
  byte *pbVar9;
  char *pcVar10;
  byte *pbVar11;
  undefined1 unaff_C_;
  undefined1 *puVar12;
  
  bVar3 = DAT_ram_e002;
  if ((((DAT_ram_e002 & 0x40) != 0) && (bVar3 = DAT_ram_e3b0, DAT_ram_e3b0 == 0)) &&
     (bVar3 = DAT_ram_e600, DAT_ram_e600 == 0)) {
    bVar3 = func_0x0141(6);
    bVar3 = ~bVar3;
    bVar5 = (bVar3 ^ DAT_ram_e06b) & bVar3;
    DAT_ram_e06b = bVar3;
    if (DAT_ram_e06a != '\0') {
      if ((bVar5 & 0x20) != 0) {
        DAT_ram_e06a = 0;
        DAT_ram_e60c = DAT_ram_e618;
        _DAT_ram_e06c = CONCAT11(DAT_ram_e06c_1,0x3c);
        FUN_ram_43bc(0x100,0x1d00,0xee60);
        func_0x0056(0xc3,0x80,0x3b00);
                    /* WARNING: Subroutine does not return */
        FUN_ram_4901(0xa8);
      }
      puVar12 = &DAT_ram_e600;
      pcVar10 = &DAT_ram_5982;
      func_0x0053(0x3b00);
      uVar6 = CONCAT11(9,DAT_ram_0007);
      do {
        *(char *)(uVar6 & 0xff) = *pcVar10 + puVar12[2];
        bVar3 = puVar12[4];
        if (bVar3 < 8) {
          bVar3 = 8;
        }
        *(byte *)(uVar6 & 0xff) = bVar3 + pcVar10[1];
        cVar8 = (char)(uVar6 >> 8);
        *(char *)(uVar6 & 0xff) = ('\t' - cVar8) * '\x04' + -100;
        *(char *)(uVar6 & 0xff) = pcVar10[2];
        pcVar10 = pcVar10 + 3;
        cVar8 = cVar8 + -1;
        uVar6 = CONCAT11(cVar8,(char)uVar6);
      } while (cVar8 != '\0');
      bVar3 = (byte)_DAT_ram_e06c | (byte)((ushort)_DAT_ram_e06c >> 8);
      if (bVar3 == 0) {
        func_0x0053(0,0x3b24);
        *(char *)(uVar6 & 0xff) = puVar12[2] + -0x12;
        bVar3 = puVar12[4];
        if (bVar3 < 8) {
          bVar3 = 8;
        }
        *(byte *)(uVar6 & 0xff) = bVar3;
        pcVar10 = &DAT_ram_599d;
        FUN_ram_4060(DAT_ram_e003 >> 4 & 7);
        *(char *)(uVar6 & 0xff) = *pcVar10;
        bVar3 = 0;
        if (*pcVar10 != '\0') {
          bVar3 = 0xf;
        }
        *(byte *)(uVar6 & 0xff) = bVar3;
        return bVar3;
      }
      _DAT_ram_e06c = _DAT_ram_e06c + -1;
      return bVar3;
    }
    bVar3 = 0;
    if ((bVar5 & 0x20) != 0) {
      DAT_ram_e06a = 1;
      DAT_ram_e667 = 1;
      DAT_ram_e618 = DAT_ram_e60c;
      _DAT_ram_e06c = 600;
      DAT_ram_e003 = 0x58;
      func_0x004d(0xc3,0x3b24);
      func_0x004d(0xd0,0x3b28);
      uVar4 = 1;
      func_0x0059(0x100,0x1d00,0xee60);
      FUN_ram_440c(0x59ed);
      FUN_ram_4478(CONCAT11(uVar4,4),0x1d00,0x1d90);
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0xa8);
    }
  }
  FUN_ram_43b0(bVar3,0x3b20);
  LAB_ram_e300 = LAB_ram_e300 + -1;
  if (LAB_ram_e300 < '\0') {
    LAB_ram_e300 = '\v';
  }
  cVar8 = LAB_ram_e300 * '\x06' + 0xa5;
  pbVar9 = (byte *)CONCAT11(0x59,cVar8);
  if (0x5a < (byte)(LAB_ram_e300 * '\x06')) {
    pbVar9 = (byte *)CONCAT11(0x5a,cVar8);
  }
  uVar6 = CONCAT11(0xc,unaff_C_);
  do {
    bVar3 = *pbVar9;
    if ((uVar6 & 0x100) != 0) {
      pbVar9 = pbVar9 + 1;
      bVar3 = bVar3 >> 4;
    }
    bVar3 = (bVar3 & 0xf) * '\b';
    cVar8 = bVar3 + 0x20;
    puVar12 = (undefined1 *)CONCAT11(0xe3,cVar8);
    if (0xdf < bVar3) {
      puVar12 = (undefined1 *)CONCAT11(0xe4,cVar8);
    }
    uVar7 = CONCAT11(8,(char)uVar6);
    do {
      puVar2 = (undefined1 *)(uVar7 & 0xff);
      cVar8 = (char)(uVar7 >> 8) + -1;
      uVar7 = CONCAT11(cVar8,(char)uVar7);
      *puVar2 = *puVar12;
      puVar12 = puVar12 + 1;
    } while (cVar8 != '\0');
    cVar8 = (char)(uVar6 >> 8) + -1;
    uVar6 = CONCAT11(cVar8,(char)uVar6);
  } while (cVar8 != '\0');
  bVar3 = DAT_ram_e06c;
  if (DAT_ram_e06c != 0) {
    _DAT_ram_e06c = CONCAT11(DAT_ram_e06c_1,DAT_ram_e06c - 1);
    return bVar3;
  }
  if (DAT_ram_e600 < 2) {
    if (DAT_ram_e668 != '\0') {
      DAT_ram_e668 = 0;
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0x18);
    }
    if ((DAT_ram_e667 != '\0') && (DAT_ram_e012 == '\0')) {
      DAT_ram_e667 = '\0';
      if (DAT_ram_e663 != '\0') {
        uVar4 = 0x18;
        if (DAT_ram_e663 != '\x01') {
          uVar4 = 0x19;
        }
                    /* WARNING: Subroutine does not return */
        FUN_ram_4901(uVar4);
      }
      FUN_ram_568c();
    }
  }
  DAT_ram_e090 = DAT_ram_e090 + 1;
  if (DAT_ram_e663 == '\0') {
    if ((DAT_ram_e090 & 0x3f) == 0) {
      FUN_ram_6459(0);
    }
    if ((DAT_ram_e091 < 0xd8) && ((byte)(DAT_ram_e090 + 0xe) < 0xd)) {
      FUN_ram_64ff();
    }
  }
  else {
    pbVar9 = &DAT_ram_e4f5;
    FUN_ram_405b(DAT_ram_e664 * '\x10');
    FUN_ram_58be();
    bVar5 = *pbVar9;
    bVar1 = ((bVar5 & 0xf) - 1 & 0x10) != 0;
    bVar3 = BCDadjust(bVar5 - 1,bVar5 == 0,bVar1);
    bVar5 = BCDadjustCarry(bVar3,bVar5 == 0,bVar1);
    hasEvenParity(bVar3);
    *pbVar9 = bVar3;
    if ((bVar5 & 1) != 0) {
      *pbVar9 = 0x59;
      pbVar11 = pbVar9 + 1;
      bVar5 = *pbVar11;
      bVar1 = ((bVar5 & 0xf) - 1 & 0x10) != 0;
      bVar3 = BCDadjust(bVar5 - 1,bVar5 == 0,bVar1);
      bVar5 = BCDadjustCarry(bVar3,bVar5 == 0,bVar1);
      hasEvenParity(bVar3);
      *pbVar11 = bVar3;
      if ((bool)(bVar5 & 1)) {
        *pbVar11 = 0;
        *pbVar9 = 0;
        FUN_ram_58be();
        DAT_ram_e090 = pbVar9[2];
        pbVar9[-1] = 0;
        DAT_ram_e663 = '\0';
        if (DAT_ram_e600 < 2) {
          FUN_ram_5690();
        }
      }
      else if (bVar3 == 4) {
        DAT_ram_e663 = 2;
                    /* WARNING: Subroutine does not return */
        FUN_ram_4901(0x19);
      }
    }
  }
  FUN_ram_88e7();
  FUN_ram_5d02();
  FUN_ram_58d3();
  if (2 < DAT_ram_e600) {
    return DAT_ram_e600;
  }
  if ((DAT_ram_e090 & 1) != 0) {
    FUN_ram_741d(DAT_ram_e090 >> 1 | 0x80);
    FUN_ram_81ba();
    bVar3 = FUN_ram_658d();
    return bVar3;
  }
  if (DAT_ram_e091 < 0xd8) {
    cVar8 = '\b';
    do {
      FUN_ram_6862();
      cVar8 = cVar8 + -1;
    } while (cVar8 != '\0');
  }
  FUN_ram_62ec();
  if ((DAT_ram_e090 & 0x3f) == 0) {
    return 0;
  }
  if (DAT_ram_e3b0 != 0) {
    return DAT_ram_e3b0;
  }
  bVar3 = FUN_ram_78e6(0);
  return bVar3;
}



/* ram:58be FUN_ram_58be -- automatic, unverified */

void FUN_ram_58be(short param_1)

{
  short sVar1;
  
  sVar1 = 0x3838;
  FUN_ram_4346(0x3838,param_1 + 1);
  func_0x004d(0xa2);
  FUN_ram_4346(sVar1 + 1);
  return;
}



/* ram:58d3 FUN_ram_58d3 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x492b) */
/* WARNING: Removing unreachable block (ram,0x4930) */
/* WARNING: Removing unreachable block (ram,0x4937) */
/* WARNING: Removing unreachable block (ram,0x493a) */
/* WARNING: Removing unreachable block (ram,0x493f) */

byte FUN_ram_58d3(undefined2 param_1)

{
  byte bVar1;
  undefined1 uVar2;
  char cVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  
  if (1 < DAT_ram_e600) {
    return DAT_ram_e600;
  }
  if (DAT_ram_e660 != '\0') {
    DAT_ram_e600 = 3;
    DAT_ram_e60b = 0;
  }
  if (DAT_ram_e091 != 0xcf) {
    return DAT_ram_e091;
  }
  if (DAT_ram_e3b0 == '\0') {
    DAT_ram_e3b0 = '\x01';
  }
  bVar1 = DAT_ram_e090 + 0x40;
  if (1 < bVar1) {
    return bVar1;
  }
  if (bVar1 != 0) {
    bVar1 = FUN_ram_569d();
    return bVar1;
  }
  disableMaskableInterrupts();
  if (((DAT_ram_e002 & 0x40) != 0) && ((DAT_ram_e012 & 0x3f) < 0x3e)) {
    puVar5 = &DAT_ram_e012;
    uVar2 = 0x3d;
    cVar3 = '\x03';
    puVar4 = &DAT_ram_4b36;
    FUN_ram_4060(0x7a,param_1,&DAT_ram_e090,&DAT_ram_e3b0);
    puVar5 = puVar5 + -2;
    do {
      *puVar5 = 1;
      puVar5[2] = uVar2;
      puVar5[3] = *puVar4;
      puVar5 = puVar5 + 4;
      puVar4 = puVar4 + 1;
      *puVar5 = *puVar4;
      FUN_ram_405b(5);
      *puVar5 = 0;
      FUN_ram_405b(5);
      puVar4 = puVar4 + 1;
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
  }
  enableMaskableInterrupts();
  return 0x3d;
}



/* ram:5a46 FUN_ram_5a46 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_5a46(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  FUN_ram_565c();
  puVar3 = &DAT_ram_e070;
  puVar2 = &DAT_ram_e071;
  sVar1 = 0x6e0;
  DAT_ram_e070 = 0;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  DAT_ram_e061 = 0;
  FUN_ram_431f();
  FUN_ram_53d3();
  DAT_ram_e061 = DAT_ram_e062 + '\x01';
  _DAT_ram_e00b = 0x800;
  DAT_ram_e064 = 1;
  return;
}



/* ram:5a73 FUN_ram_5a73 -- automatic, unverified */

void FUN_ram_5a73(void)

{
  byte bVar1;
  byte bVar2;
  byte *pbVar3;
  
  DAT_ram_e00c = DAT_ram_e00c + -1;
  if (DAT_ram_e00c == '\0') {
    DAT_ram_e00c = '\b';
    DAT_ram_e00b = DAT_ram_e00b + '\x01';
  }
  pbVar3 = &DAT_ram_5a95;
  FUN_ram_405b(DAT_ram_e00b);
  bVar1 = DAT_ram_e009;
  bVar2 = *pbVar3;
  if (bVar2 == 0xff) {
    DAT_ram_e064 = 0;
    FUN_ram_490c(0x3d);
    return;
  }
  DAT_ram_e009 = bVar2;
  DAT_ram_e008 = (bVar2 ^ bVar1) & bVar2;
  return;
}



/* ram:5b0b FUN_ram_5b0b -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_5b0b(undefined2 param_1)

{
  byte in_F;
  byte bVar1;
  ushort uVar2;
  char cVar3;
  char *pcVar4;
  undefined1 *puVar5;
  short sVar6;
  undefined1 *puVar7;
  
  cVar3 = (char)((ushort)param_1 >> 8);
  if (cVar3 == '\x01') {
    DAT_ram_e004 = DAT_ram_e004 + -1;
    if (DAT_ram_e004 != '\0') {
      return;
    }
    DAT_ram_e602 = 0x97;
    DAT_ram_e604 = 0x78;
    DAT_ram_e682 = 0;
    DAT_ram_e684 = 0x78;
    _DAT_ram_e676 = 0x70;
    FUN_ram_6112();
    FUN_ram_431f();
    FUN_ram_64a3();
  }
  else if (cVar3 == '\x02') {
    FUN_ram_60e1(&DAT_ram_e600);
    sVar6 = -0x1980;
    FUN_ram_60eb();
    pcVar4 = &DAT_ram_5c74;
    func_0x0053(0x3b30);
    uVar2 = CONCAT11(4,DAT_ram_0007);
    do {
      *(char *)(uVar2 & 0xff) = *pcVar4 + *(char *)(sVar6 + 2);
      *(char *)(uVar2 & 0xff) = pcVar4[1] + *(char *)(sVar6 + 4);
      cVar3 = (char)(uVar2 >> 8);
      *(char *)(uVar2 & 0xff) = ('\x04' - cVar3) * '\x04' + '\f';
      *(char *)(uVar2 & 0xff) = pcVar4[2];
      pcVar4 = pcVar4 + 3;
      cVar3 = cVar3 + -1;
      uVar2 = CONCAT11(cVar3,(char)uVar2);
    } while (cVar3 != '\0');
    if (*(byte *)(sVar6 + 2) < 0x48) {
      return;
    }
    _DAT_ram_e676 = 0xff60;
  }
  else if (cVar3 == '\x03') {
    puVar7 = &DAT_ram_e600;
    FUN_ram_60db();
    if (0x5a < (byte)puVar7[2]) {
      return;
    }
    DAT_ram_e690 = 'x';
  }
  else if (cVar3 == '\x04') {
    DAT_ram_e690 = DAT_ram_e690 + -1;
    if (DAT_ram_e690 != '\0') {
      return;
    }
    puVar5 = &DAT_ram_5c80;
    puVar7 = &DAT_ram_e6a0;
    sVar6 = 5;
    DAT_ram_e690 = '\0';
    do {
      *puVar7 = *puVar5;
      puVar7 = puVar7 + 1;
      puVar5 = puVar5 + 1;
      sVar6 = sVar6 + -1;
    } while (sVar6 != 0);
  }
  else if (cVar3 == '\x05') {
    bVar1 = in_F & 0xec | DAT_ram_e003 & 1;
    if ((bool)(DAT_ram_e003 & 1)) {
      return;
    }
    FUN_ram_5c06(DAT_ram_e003 >> 1 | in_F << 7);
    if (-1 < (char)bVar1) {
      return;
    }
    FUN_ram_43f1(0x5ca4);
  }
  else {
    if (cVar3 == '\x06') {
      DAT_ram_e690 = DAT_ram_e690 + -1;
      if (DAT_ram_e690 == '\0') {
        puVar5 = &DAT_ram_e080;
        puVar7 = &DAT_ram_e609;
        sVar6 = 7;
        DAT_ram_e690 = 0;
        do {
          *puVar7 = *puVar5;
          puVar7 = puVar7 + 1;
          puVar5 = puVar5 + 1;
          sVar6 = sVar6 + -1;
        } while (sVar6 != 0);
        DAT_ram_e000 = 4;
        DAT_ram_e004 = 0x20;
        DAT_ram_e001 = 0;
        return;
      }
      return;
    }
    if (DAT_ram_e012 != '\0') {
      return;
    }
    FUN_ram_4386(0);
    FUN_ram_490c(0xb4);
    puVar5 = &DAT_ram_e609;
    puVar7 = &DAT_ram_e080;
    sVar6 = 7;
    do {
      *puVar7 = *puVar5;
      puVar7 = puVar7 + 1;
      puVar5 = puVar5 + 1;
      sVar6 = sVar6 + -1;
    } while (sVar6 != 0);
    DAT_ram_e60c = 0;
    FUN_ram_45b5();
    FUN_ram_440c(0x5cb9);
    FUN_ram_43f1(0x5c85);
    DAT_ram_e004 = -0x4c;
  }
  DAT_ram_e001 = DAT_ram_e001 + '\x01';
  return;
}



/* ram:5c06 FUN_ram_5c06 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_ram_5c06(void)

{
  char cVar1;
  short sVar2;
  undefined2 uVar3;
  undefined3 uVar4;
  undefined2 in_AF;
  byte bVar5;
  byte bVar7;
  ushort uVar6;
  
  uVar4 = _DAT_ram_e6a2;
  cVar1 = DAT_ram_e6a4 + -1;
  uVar3 = _DAT_ram_e6a2;
  _DAT_ram_e6a2 = CONCAT12(cVar1,_DAT_ram_e6a2);
  if (cVar1 < '\0') {
    return (char)((ushort)in_AF >> 8);
  }
  bVar5 = (byte)uVar4;
  bVar7 = (byte)((uint3)uVar4 >> 8);
  if (bVar7 <= bVar5) {
    FUN_ram_5c67();
    FUN_ram_405b(uVar3);
    FUN_ram_5c67();
    _DAT_ram_e6a0 = _DAT_ram_e6a0 + 1;
    _DAT_ram_e6a2 = CONCAT21(_DAT_ram_e6a3,DAT_ram_e6a2 + -2);
    if (bVar5 != bVar7) {
      return 0;
    }
  }
  sVar2 = _DAT_ram_e6a0;
  uVar6 = _DAT_ram_e6a2 & 0xff;
  func_0x0056();
  func_0x0056(uVar6 + 1,(ushort)(byte)((char)((uint3)_DAT_ram_e6a2 >> 8) - 1) * 0x20 + sVar2);
  _DAT_ram_e6a2 = CONCAT11(DAT_ram_e6a3 + -2,DAT_ram_e6a2);
  _DAT_ram_e6a0 = _DAT_ram_e6a0 + 0x20;
  return 0;
}



/* ram:5c67 FUN_ram_5c67 -- automatic, unverified */

void FUN_ram_5c67(undefined2 param_1,short param_2)

{
  undefined1 uVar1;
  char cVar2;
  short sVar3;
  short sVar4;
  
  cVar2 = (char)((ushort)param_1 >> 8);
  uVar1 = 0;
  sVar3 = 0x20;
  sVar4 = param_2;
  do {
    uVar1 = func_0x004d(uVar1,param_2,sVar4);
    param_2 = param_2 + sVar3;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:5d02 FUN_ram_5d02 -- automatic, unverified */

void FUN_ram_5d02(void)

{
                    /* WARNING: Subroutine does not return */
  inline_dispatch(DAT_ram_e600);
}



/* ram:5d14 state_5d14 -- automatic, unverified */

void state_5d14(undefined1 *param_1)

{
  FUN_ram_5ef1();
  if ((param_1[0xc] != '\x03') && ((DAT_ram_e008 & 0x10) != 0)) {
    *param_1 = 1;
    param_1[0xb] = 0;
  }
  FUN_ram_5dff();
  return;
}



/* ram:5d30 state_5d30 -- automatic, unverified */

void state_5d30(undefined1 *param_1)

{
  byte bVar1;
  
  FUN_ram_5ef1();
  param_1[0xb] = param_1[0xb] + '\x01';
  bVar1 = param_1[0xb];
  if (bVar1 == 4) {
    bVar1 = FUN_ram_6026();
  }
  if (5 < bVar1) {
    *param_1 = 0;
    return;
  }
  FUN_ram_5dff();
  return;
}



/* ram:5d47 state_5d47 -- automatic, unverified */

/* WARNING: Control flow encountered bad instruction data */

void state_5d47(short param_1)

{
  byte bVar1;
  char cVar2;
  undefined1 uVar3;
  
  cVar2 = *(char *)(param_1 + 0xb) + -1;
  *(char *)(param_1 + 0xb) = cVar2;
  bVar1 = *(byte *)(param_1 + 0xb);
  if (cVar2 == '\0') {
    DAT_ram_e064 = bVar1;
    return;
  }
  if ((bVar1 < 0x51) &&
     ((((uVar3 = 0, 0x48 < bVar1 || (uVar3 = 1, 0x40 < bVar1)) || (uVar3 = 2, 0x38 < bVar1)) ||
      (uVar3 = 3, 0x30 < bVar1)))) {
    *(undefined1 *)(param_1 + 6) = uVar3;
    *(undefined1 *)(param_1 + 0xc) = 4;
    FUN_ram_5dfa(0xa60b);
    return;
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ram:5d95 state_5d95 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte state_5d95(undefined1 param_1,undefined2 param_2,short param_3)

{
  byte bVar1;
  char cVar3;
  undefined2 uVar2;
  
  if (*(char *)(param_3 + 0xb) != '\x01') {
    cVar3 = *(char *)(param_3 + 0xb) + -2;
    uVar2 = CONCAT11(cVar3,param_1);
    if (cVar3 != '\0') {
      *(char *)(param_3 + 0xb) = *(char *)(param_3 + 0xb) + '\x01';
      FUN_ram_6112();
      DAT_ram_e675 = FUN_ram_6226();
      FUN_ram_83e1();
      if (((DAT_ram_e675 & 0x7f) < 5) || (0x7a < (DAT_ram_e675 & 0x7f))) {
        DAT_ram_e675 = 0;
        uVar2 = 0;
      }
      _DAT_ram_e676 = uVar2;
      _DAT_ram_e678 = param_2;
      bVar1 = FUN_ram_490c(0xb7);
      return bVar1;
    }
    bVar1 = FUN_ram_60db();
    if (4 < bVar1) {
      return bVar1;
    }
    DAT_ram_e00d = 1;
    return 1;
  }
  bVar1 = FUN_ram_60db();
  if (0x1f < bVar1) {
    if ((DAT_ram_e675 & 0x7f) != 0) {
      return DAT_ram_e675 & 0x7f;
    }
    bVar1 = *(byte *)(param_3 + 4);
    if (bVar1 != 0x78) {
      return bVar1;
    }
  }
  *(char *)(param_3 + 0xb) = *(char *)(param_3 + 0xb) + '\x01';
  _DAT_ram_e678 = 0;
  _DAT_ram_e676 = 0xff50;
  return bVar1;
}



/* ram:5dfa FUN_ram_5dfa -- automatic, unverified */

void FUN_ram_5dfa(char *param_1,short param_2)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  ushort uVar5;
  byte *pbVar6;
  byte *pbVar7;
  
  func_0x0053(0x3b00);
  uVar5 = CONCAT11(3,DAT_ram_0007);
  do {
    uVar4 = uVar5;
    uVar5 = uVar4;
    FUN_ram_5ed5();
    *(char *)(uVar5 & 0xff) = ('\x03' - (char)(uVar5 >> 8)) * '\x04';
    cVar1 = *param_1;
    if (((*(char *)(param_2 + 0xc) == '\x03') && (cVar1 == '\a')) &&
       ((0x11 < *(byte *)(param_2 + 0xe) || (cVar1 = *param_1, (DAT_ram_e003 & 4) == 0)))) {
      cVar1 = '\x06';
    }
    *(char *)(uVar5 & 0xff) = cVar1;
    param_1 = param_1 + 1;
    cVar1 = (char)(uVar4 >> 8) + -1;
    uVar5 = CONCAT11(cVar1,(char)uVar4);
  } while (cVar1 != '\0');
  if (*(char *)(param_2 + 0xc) == '\x01') {
    param_1 = &DAT_ram_a5da;
    FUN_ram_5ed5(0);
    *(char *)(uVar5 & 0xff) = *param_1;
    uVar3 = (undefined1)uVar5;
    uVar5 = CONCAT11(7,uVar3);
    if ((*(byte *)(param_2 + 0xf) < 10) &&
       (uVar5 = CONCAT11(10,uVar3), *(byte *)(param_2 + 0xf) < 5)) {
      uVar5 = CONCAT11(9,uVar3);
    }
    *(char *)(uVar5 & 0xff) = (char)(uVar5 >> 8);
  }
  else {
    *(undefined1 *)(uVar4 & 0xff) = 0xc3;
  }
  FUN_ram_4079(*(byte *)(param_2 + 0xc) & 0xf,0xa5b4);
  FUN_ram_4071(*(undefined1 *)(param_2 + 6));
  if ((*(byte *)(param_2 + 0xc) & 0x80) != 0) {
    *(undefined1 *)(param_2 + 0xc) = 2;
  }
  func_0x0053(0x1800);
  while (cVar1 = *param_1, cVar1 != '\0') {
    pbVar7 = *(byte **)(param_1 + 1);
    param_1 = param_1 + 3;
    if (cVar1 == -1) {
      while (bVar2 = *pbVar7, bVar2 != 0) {
        pbVar6 = pbVar7 + 1;
        uVar5 = CONCAT11(bVar2,(char)uVar5) & 0x7fff;
        if ((bVar2 & 0x7f) == bVar2) {
          pbVar7 = pbVar7 + 2;
          do {
            *(byte *)(uVar5 & 0xff) = *pbVar6;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
          } while (cVar1 != '\0');
        }
        else {
          do {
            pbVar7 = pbVar6 + 1;
            *(byte *)(uVar5 & 0xff) = *pbVar6;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
            pbVar6 = pbVar7;
          } while (cVar1 != '\0');
        }
      }
    }
    else {
      uVar5 = CONCAT11(0x20,(char)uVar5);
      do {
        bVar2 = *pbVar7;
        pbVar7 = pbVar7 + 1;
        *(byte *)(uVar5 & 0xff) = bVar2;
        cVar1 = (char)(uVar5 >> 8) + -1;
        uVar5 = CONCAT11(cVar1,(char)uVar5);
      } while (cVar1 != '\0');
    }
  }
  return;
}



/* ram:5dff FUN_ram_5dff -- automatic, unverified */

void FUN_ram_5dff(short param_1)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  ushort uVar5;
  char *pcVar6;
  byte *pbVar7;
  byte *pbVar8;
  
  pcVar6 = &DAT_ram_a5c2;
  if (((*(char *)(param_1 + 0xc) == '\x02') &&
      (pcVar6 = (char *)0xa5ef, *(byte *)(param_1 + 0xe) < 0x12)) && ((DAT_ram_e003 & 8) != 0)) {
    pcVar6 = &DAT_ram_a5c2;
    *(undefined1 *)(param_1 + 0xc) = 0x80;
  }
  func_0x0053(0x3b00);
  uVar5 = CONCAT11(3,DAT_ram_0007);
  do {
    uVar4 = uVar5;
    uVar5 = uVar4;
    FUN_ram_5ed5();
    *(char *)(uVar5 & 0xff) = ('\x03' - (char)(uVar5 >> 8)) * '\x04';
    cVar1 = *pcVar6;
    if (((*(char *)(param_1 + 0xc) == '\x03') && (cVar1 == '\a')) &&
       ((0x11 < *(byte *)(param_1 + 0xe) || (cVar1 = *pcVar6, (DAT_ram_e003 & 4) == 0)))) {
      cVar1 = '\x06';
    }
    *(char *)(uVar5 & 0xff) = cVar1;
    pcVar6 = pcVar6 + 1;
    cVar1 = (char)(uVar4 >> 8) + -1;
    uVar5 = CONCAT11(cVar1,(char)uVar4);
  } while (cVar1 != '\0');
  if (*(char *)(param_1 + 0xc) == '\x01') {
    pcVar6 = &DAT_ram_a5da;
    FUN_ram_5ed5(0);
    *(char *)(uVar5 & 0xff) = *pcVar6;
    uVar3 = (undefined1)uVar5;
    uVar5 = CONCAT11(7,uVar3);
    if ((*(byte *)(param_1 + 0xf) < 10) &&
       (uVar5 = CONCAT11(10,uVar3), *(byte *)(param_1 + 0xf) < 5)) {
      uVar5 = CONCAT11(9,uVar3);
    }
    *(char *)(uVar5 & 0xff) = (char)(uVar5 >> 8);
  }
  else {
    *(undefined1 *)(uVar4 & 0xff) = 0xc3;
  }
  FUN_ram_4079(*(byte *)(param_1 + 0xc) & 0xf,0xa5b4);
  FUN_ram_4071(*(undefined1 *)(param_1 + 6));
  if ((*(byte *)(param_1 + 0xc) & 0x80) != 0) {
    *(undefined1 *)(param_1 + 0xc) = 2;
  }
  func_0x0053(0x1800);
  while (cVar1 = *pcVar6, cVar1 != '\0') {
    pbVar8 = *(byte **)(pcVar6 + 1);
    pcVar6 = pcVar6 + 3;
    if (cVar1 == -1) {
      while (bVar2 = *pbVar8, bVar2 != 0) {
        pbVar7 = pbVar8 + 1;
        uVar5 = CONCAT11(bVar2,(char)uVar5) & 0x7fff;
        if ((bVar2 & 0x7f) == bVar2) {
          pbVar8 = pbVar8 + 2;
          do {
            *(byte *)(uVar5 & 0xff) = *pbVar7;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
          } while (cVar1 != '\0');
        }
        else {
          do {
            pbVar8 = pbVar7 + 1;
            *(byte *)(uVar5 & 0xff) = *pbVar7;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
            pbVar7 = pbVar8;
          } while (cVar1 != '\0');
        }
      }
    }
    else {
      uVar5 = CONCAT11(0x20,(char)uVar5);
      do {
        bVar2 = *pbVar8;
        pbVar8 = pbVar8 + 1;
        *(byte *)(uVar5 & 0xff) = bVar2;
        cVar1 = (char)(uVar5 >> 8) + -1;
        uVar5 = CONCAT11(cVar1,(char)uVar5);
      } while (cVar1 != '\0');
    }
  }
  return;
}



/* ram:5e02 FUN_ram_5e02 -- automatic, unverified */

void FUN_ram_5e02(short param_1)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  ushort uVar4;
  ushort uVar5;
  char *pcVar6;
  byte *pbVar7;
  byte *pbVar8;
  
  pcVar6 = &DAT_ram_a5c2;
  if (((*(char *)(param_1 + 0xc) == '\x02') &&
      (pcVar6 = (char *)0xa5ef, *(byte *)(param_1 + 0xe) < 0x12)) && ((DAT_ram_e003 & 8) != 0)) {
    pcVar6 = &DAT_ram_a5c2;
    *(undefined1 *)(param_1 + 0xc) = 0x80;
  }
  func_0x0053();
  uVar5 = CONCAT11(3,DAT_ram_0007);
  do {
    uVar4 = uVar5;
    uVar5 = uVar4;
    FUN_ram_5ed5();
    *(char *)(uVar5 & 0xff) = ('\x03' - (char)(uVar5 >> 8)) * '\x04';
    cVar1 = *pcVar6;
    if (((*(char *)(param_1 + 0xc) == '\x03') && (cVar1 == '\a')) &&
       ((0x11 < *(byte *)(param_1 + 0xe) || (cVar1 = *pcVar6, (DAT_ram_e003 & 4) == 0)))) {
      cVar1 = '\x06';
    }
    *(char *)(uVar5 & 0xff) = cVar1;
    pcVar6 = pcVar6 + 1;
    cVar1 = (char)(uVar4 >> 8) + -1;
    uVar5 = CONCAT11(cVar1,(char)uVar4);
  } while (cVar1 != '\0');
  if (*(char *)(param_1 + 0xc) == '\x01') {
    pcVar6 = &DAT_ram_a5da;
    FUN_ram_5ed5(0);
    *(char *)(uVar5 & 0xff) = *pcVar6;
    uVar3 = (undefined1)uVar5;
    uVar5 = CONCAT11(7,uVar3);
    if ((*(byte *)(param_1 + 0xf) < 10) &&
       (uVar5 = CONCAT11(10,uVar3), *(byte *)(param_1 + 0xf) < 5)) {
      uVar5 = CONCAT11(9,uVar3);
    }
    *(char *)(uVar5 & 0xff) = (char)(uVar5 >> 8);
  }
  else {
    *(undefined1 *)(uVar4 & 0xff) = 0xc3;
  }
  FUN_ram_4079(*(byte *)(param_1 + 0xc) & 0xf,0xa5b4);
  FUN_ram_4071(*(undefined1 *)(param_1 + 6));
  if ((*(byte *)(param_1 + 0xc) & 0x80) != 0) {
    *(undefined1 *)(param_1 + 0xc) = 2;
  }
  func_0x0053(0x1800);
  while (cVar1 = *pcVar6, cVar1 != '\0') {
    pbVar8 = *(byte **)(pcVar6 + 1);
    pcVar6 = pcVar6 + 3;
    if (cVar1 == -1) {
      while (bVar2 = *pbVar8, bVar2 != 0) {
        pbVar7 = pbVar8 + 1;
        uVar5 = CONCAT11(bVar2,(char)uVar5) & 0x7fff;
        if ((bVar2 & 0x7f) == bVar2) {
          pbVar8 = pbVar8 + 2;
          do {
            *(byte *)(uVar5 & 0xff) = *pbVar7;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
          } while (cVar1 != '\0');
        }
        else {
          do {
            pbVar8 = pbVar7 + 1;
            *(byte *)(uVar5 & 0xff) = *pbVar7;
            cVar1 = (char)(uVar5 >> 8) + -1;
            uVar5 = CONCAT11(cVar1,(char)uVar5);
            pbVar7 = pbVar8;
          } while (cVar1 != '\0');
        }
      }
    }
    else {
      uVar5 = CONCAT11(0x20,(char)uVar5);
      do {
        bVar2 = *pbVar8;
        pbVar8 = pbVar8 + 1;
        *(byte *)(uVar5 & 0xff) = bVar2;
        cVar1 = (char)(uVar5 >> 8) + -1;
        uVar5 = CONCAT11(cVar1,(char)uVar5);
      } while (cVar1 != '\0');
    }
  }
  return;
}



/* ram:5ed5 FUN_ram_5ed5 -- automatic, unverified */

void FUN_ram_5ed5(byte param_1,char *param_2,short param_3)

{
  byte bVar1;
  char cVar2;
  
  bVar1 = *(byte *)(param_3 + 8) & 0xc;
  if ((bVar1 == 0xb) || (cVar2 = *param_2, bVar1 == 0xc)) {
    cVar2 = *param_2 + -1;
  }
  *(char *)(ushort)param_1 = cVar2 + *(char *)(param_3 + 2);
  *(undefined1 *)(ushort)param_1 = *(undefined1 *)(param_3 + 4);
  return;
}



/* ram:5ef1 FUN_ram_5ef1 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x5fe2) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_5ef1(short param_1)

{
  bool bVar1;
  char cVar2;
  byte bVar3;
  byte bVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  undefined1 *puVar7;
  undefined2 in_stack_00000000;
  
  if (((DAT_ram_e008 & 0x20) != 0) && ((DAT_ram_e069 & 0x1f) != 0)) {
    if (((DAT_ram_e069 & 0x10) == 0) && (DAT_ram_e069 = DAT_ram_e069 + 1, 3 < DAT_ram_e069)) {
      DAT_ram_e069 = 0;
    }
    DAT_ram_e60c = 2;
  }
  puVar7 = &DAT_ram_62e2;
  cVar2 = *(char *)(param_1 + 10);
  if (*(char *)(param_1 + 0xc) == '\x03') {
    cVar2 = '\x04';
  }
  FUN_ram_405b(cVar2 * '\x02');
  uVar6 = *puVar7;
  puVar7 = puVar7 + 1;
  uVar5 = *puVar7;
  bVar3 = DAT_ram_e009 & 0xf;
  if (bVar3 == 0) {
    bVar3 = FUN_ram_6141();
  }
  bVar4 = (byte)((ushort)puVar7 >> 8);
  if ((bVar3 & 3) != 0) {
    _DAT_ram_e074 = *(undefined2 *)(param_1 + 1);
    FUN_ram_6003(bVar3 & 3);
    *(undefined1 *)(param_1 + 1) = uVar6;
    if (bVar4 < 0x21) {
      bVar4 = 0x20;
    }
    if (0x9f < bVar4) {
      bVar4 = 0x9f;
    }
    *(byte *)(param_1 + 2) = bVar4;
    if ((DAT_ram_e009 & 0xc) == 0) {
      FUN_ram_6155(0);
    }
  }
  if ((DAT_ram_e009 & 0xc) != 0) {
    _DAT_ram_e076 = *(undefined2 *)(param_1 + 3);
    FUN_ram_6003((DAT_ram_e009 & 0xc) >> 2);
    *(undefined1 *)(param_1 + 3) = uVar5;
    if (bVar4 < 4) {
      bVar4 = 3;
    }
    if (0xef < bVar4) {
      bVar4 = 0xef;
    }
    *(byte *)(param_1 + 4) = bVar4;
    if ((DAT_ram_e009 & 0xc) != 0) {
      FUN_ram_6155(DAT_ram_e009 & 0xc);
    }
  }
  FUN_ram_6014();
  FUN_ram_6163();
  if ((((DAT_ram_e3b0 == '\0') || (DAT_ram_e3c0 != '\0')) ||
      ((bVar3 = DAT_ram_e062 < 6, bVar4 = DAT_ram_e062, DAT_ram_e062 == 6 &&
       (bVar3 = false, bVar4 = DAT_ram_e3d0, DAT_ram_e3d0 == 0)))) ||
     (FUN_ram_875e(bVar4), (bVar3 & 1) != 0)) {
    if (*(byte *)(param_1 + 2) < 0xa0) {
      if (*(byte *)(param_1 + 0xc) < 2) {
        return;
      }
      FUN_ram_5ff8();
      cVar2 = *(char *)(param_1 + 0xd) + -1;
      *(char *)(param_1 + 0xd) = cVar2;
      if (cVar2 == '\0') {
        *(undefined1 *)(param_1 + 0xd) = 0x14;
                    /* WARNING: Subroutine does not return */
        if ((*(byte *)(param_1 + 0xe) != 0) && (*(byte *)(param_1 + 0xe) < 0x12)) {
          FUN_ram_4901(0x13);
        }
        bVar3 = *(byte *)(param_1 + 0xe);
        bVar1 = ((bVar3 & 0xf) - 1 & 0x10) != 0;
        cVar2 = BCDadjust(bVar3 - 1,bVar3 == 0,bVar1);
        BCDadjustCarry(cVar2,bVar3 == 0,bVar1);
        bVar3 = hasEvenParity(cVar2);
        *(char *)(param_1 + 0xe) = cVar2;
        if (cVar2 == '\0' || (bVar3 & 0x10) != 0) {
          *(bool *)(param_1 + 0xc) = *(char *)(param_1 + 0xf) != '\0';
                    /* WARNING: Subroutine does not return */
          FUN_ram_4901(0x14);
        }
        return;
      }
      return;
    }
    *(undefined1 *)(param_1 + 2) = 0x9f;
  }
  FUN_ram_8338(in_stack_00000000);
  return;
}



/* ram:5ff8 FUN_ram_5ff8 -- automatic, unverified */

void FUN_ram_5ff8(void)

{
  FUN_ram_4346(0x3824,0xe60e);
  return;
}



/* ram:6003 FUN_ram_6003 -- automatic, unverified */

byte FUN_ram_6003(byte param_1,byte param_2,ushort param_3)

{
  ushort uVar1;
  byte bVar2;
  
  uVar1 = (ushort)param_2 * 8;
  bVar2 = (byte)(param_1 >> 1 | CARRY2(uVar1,uVar1) << 7) >> 1 |
          ((param_1 & 1) != 0 && param_3 < (ushort)param_2 * 0x10) << 7;
  if ((param_1 >> 1 & 1) == 0) {
    return bVar2;
  }
  return bVar2;
}



/* ram:6014 FUN_ram_6014 -- automatic, unverified */

void FUN_ram_6014(short param_1)

{
  *(undefined1 *)(param_1 + 5) = *(undefined1 *)(param_1 + 6);
  *(byte *)(param_1 + 6) = *(byte *)(param_1 + 8) >> 3 & 1;
  return;
}



/* ram:6017 FUN_ram_6017 -- automatic, unverified */

void FUN_ram_6017(byte param_1,short param_2)

{
  *(undefined1 *)(param_2 + 5) = *(undefined1 *)(param_2 + 6);
  *(byte *)(param_2 + 6) = param_1 >> 3 & 1;
  return;
}



/* ram:6026 FUN_ram_6026 -- automatic, unverified */

void FUN_ram_6026(short param_1)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  char *pcVar4;
  byte *pbVar5;
  undefined1 *puVar6;
  
  bVar1 = *(byte *)(param_1 + 9);
  pcVar4 = &DAT_ram_60bd;
  FUN_ram_405b(bVar1 * '\x02');
  cVar2 = *pcVar4;
  sVar3 = (ushort)(byte)pcVar4[1] << 8;
  pbVar5 = &DAT_ram_e620;
  while (((*pbVar5 & 0x80) == 0 ||
         ((bVar1 >> 1 == 4 && (!(bool)((pbVar5[0x10] | pbVar5[0x20]) >> 7)))))) {
    FUN_ram_405b(0x10);
    cVar2 = cVar2 + -1;
    if (cVar2 == '\0') {
      return;
    }
  }
  *pbVar5 = *pbVar5 & 0x7f;
  *pbVar5 = *pbVar5 & 0xef;
  pbVar5[2] = *(char *)(param_1 + 2) - 8;
  pbVar5[4] = *(char *)(param_1 + 4) + (char)((ushort)sVar3 >> 8);
  pbVar5[5] = bVar1;
  pbVar5[6] = 0;
  pbVar5[7] = 0;
  pbVar5[8] = 0x42;
  pbVar5[9] = 100;
  puVar6 = &DAT_ram_60d5;
  FUN_ram_405b(bVar1 >> 1);
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(*puVar6);
}



/* ram:60db FUN_ram_60db -- automatic, unverified */

undefined1 FUN_ram_60db(short param_1)

{
  FUN_ram_60eb();
  FUN_ram_6017();
  FUN_ram_5e02(0x3b20);
  return *(undefined1 *)(param_1 + 2);
}



/* ram:60e1 FUN_ram_60e1 -- automatic, unverified */

undefined1 FUN_ram_60e1(short param_1)

{
  FUN_ram_5e02(0x3b20);
  return *(undefined1 *)(param_1 + 2);
}



/* ram:60eb FUN_ram_60eb -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_ram_60eb(short param_1)

{
  bool bVar1;
  
  *(short *)(param_1 + 3) = *(short *)(param_1 + 3) + _DAT_ram_e678;
  bVar1 = CARRY2(*(ushort *)(param_1 + 1),_DAT_ram_e676);
  *(ushort *)(param_1 + 1) = *(ushort *)(param_1 + 1) + _DAT_ram_e676;
  return DAT_ram_e003 >> 1 | bVar1 << 7;
}



/* ram:6112 FUN_ram_6112 -- automatic, unverified */

void FUN_ram_6112(void)

{
  undefined1 uVar1;
  byte bVar2;
  undefined1 *puVar3;
  short sVar4;
  undefined1 *puVar5;
  
  func_0x0056(0,0x3b00);
  sVar4 = 0x3b00;
  bVar2 = 8;
  do {
    uVar1 = 5;
    if (4 < bVar2) {
      uVar1 = 0xfc;
    }
    func_0x004d(uVar1,sVar4);
    sVar4 = sVar4 + 4;
    bVar2 = bVar2 - 1;
  } while (bVar2 != 0);
  puVar5 = &DAT_ram_e330;
  puVar3 = &DAT_ram_e331;
  sVar4 = 0x4f;
  DAT_ram_e330 = 0xc3;
  do {
    *puVar3 = *puVar5;
    puVar3 = puVar3 + 1;
    puVar5 = puVar5 + 1;
    sVar4 = sVar4 + -1;
  } while (sVar4 != 0);
  return;
}



/* ram:6121 state_6121 -- automatic, unverified */

void state_6121(short param_1,short param_2)

{
  undefined1 uVar1;
  byte bVar3;
  short sVar2;
  undefined1 *puVar4;
  undefined1 *puVar5;
  
  do {
    bVar3 = (byte)((ushort)param_1 >> 8);
    uVar1 = 5;
    if (4 < bVar3) {
      uVar1 = 0xfc;
    }
    func_0x004d(uVar1,param_2);
    param_2 = param_2 + 4;
    param_1 = (ushort)(byte)(bVar3 - 1) << 8;
  } while ((byte)(bVar3 - 1) != 0);
  puVar5 = &DAT_ram_e330;
  puVar4 = &DAT_ram_e331;
  sVar2 = 0x4f;
  DAT_ram_e330 = 0xc3;
  do {
    *puVar4 = *puVar5;
    puVar4 = puVar4 + 1;
    puVar5 = puVar5 + 1;
    sVar2 = sVar2 + -1;
  } while (sVar2 != 0);
  return;
}



/* ram:6141 FUN_ram_6141 -- automatic, unverified */

undefined1 FUN_ram_6141(ushort param_1,undefined2 param_2,short param_3)

{
  undefined2 in_AF;
  
  *(undefined1 *)(param_3 + 1) = 0;
  *(undefined1 *)(param_3 + 3) = 0;
  FUN_ram_6155(param_2,(param_1 >> 8) * 8);
  return (char)((ushort)in_AF >> 8);
}



/* ram:6155 FUN_ram_6155 -- automatic, unverified */

void FUN_ram_6155(short param_1,short param_2)

{
  *(short *)(param_2 + 7) = *(short *)(param_2 + 7) + param_1;
  return;
}



/* ram:6163 FUN_ram_6163 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x61fe) */
/* WARNING: Removing unreachable block (ram,0x6204) */
/* WARNING: Removing unreachable block (ram,0x61c0) */
/* WARNING: Removing unreachable block (ram,0x61b8) */
/* WARNING: Removing unreachable block (ram,0x619d) */
/* WARNING: Removing unreachable block (ram,0x61a5) */
/* WARNING: Removing unreachable block (ram,0x61de) */
/* WARNING: Removing unreachable block (ram,0x61e6) */
/* WARNING: Removing unreachable block (ram,0x6205) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_6163(short param_1)

{
  char cVar1;
  byte bVar2;
  undefined1 uVar3;
  char cVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined1 *puVar8;
  
  pcVar5 = &DAT_ram_6216;
  puVar8 = &DAT_ram_e610;
  cVar4 = '\b';
  do {
    cVar1 = *pcVar5;
    pcVar6 = pcVar5 + 1;
    pcVar5 = pcVar5 + 2;
    uVar7 = CONCAT11(*(char *)(param_1 + 4) + *pcVar6,*(char *)(param_1 + 2) + cVar1);
    FUN_ram_4372(uVar7);
    uVar3 = func_0x004a();
    *puVar8 = uVar3;
    puVar8 = puVar8 + 1;
    cVar4 = cVar4 + -1;
  } while (cVar4 != '\0');
  puVar8 = &DAT_ram_e610;
  bVar2 = DAT_ram_e009 >> 1;
  if ((DAT_ram_e009 & 1) != 0) {
    FUN_ram_620c(DAT_ram_e610,uVar7);
  }
  if ((bVar2 & 1) != 0) {
    FUN_ram_620c(puVar8[4]);
  }
  if ((bVar2 & 2) != 0) {
    FUN_ram_620c(puVar8[2]);
    FUN_ram_620c(*puVar8);
  }
  if ((bVar2 & 4) != 0) {
    FUN_ram_620c(puVar8[3]);
    FUN_ram_620c(puVar8[1]);
    return;
  }
  return;
}



/* ram:620c FUN_ram_620c -- automatic, unverified */

byte FUN_ram_620c(char param_1)

{
  if ((byte)(param_1 + 0xbdU) < 0x3d) {
    return param_1 + 0xbdU;
  }
  return param_1 + 0x5d;
}



/* ram:6226 FUN_ram_6226 -- automatic, unverified */

char FUN_ram_6226(byte param_1,short param_2)

{
  char cVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar4;
  ushort *puVar5;
  
  cVar1 = *(char *)(param_2 + 2) + -0x20;
  cVar4 = '\0';
  if (cVar1 != '\0') {
    cVar4 = cVar1;
  }
  cVar1 = 'x' - *(byte *)(param_2 + 4);
  uVar2 = (ushort)param_1;
  if ((cVar1 != '\0') && (0x78 < *(byte *)(param_2 + 4))) {
    uVar2 = CONCAT11(1,param_1);
    cVar1 = -cVar1;
  }
  uVar3 = uVar2;
  FUN_ram_8569(cVar1,cVar4);
  puVar5 = (ushort *)&DAT_ram_6262;
  cVar1 = '@';
  do {
    if (*puVar5 < uVar3) break;
    puVar5 = puVar5 + 1;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  if ((char)(uVar2 >> 8) == '\x01') {
    return -0x80 - cVar1;
  }
  return cVar1;
}



/* ram:62ec FUN_ram_62ec -- automatic, unverified */

void FUN_ram_62ec(void)

{
  char cVar1;
  short sVar2;
  byte *pbVar3;
  
  pbVar3 = &DAT_ram_e620;
  cVar1 = '\x03';
  do {
    if ((*pbVar3 & 0x80) == 0) {
      if ((pbVar3[5] == 8) && ((*pbVar3 & 0xf) != 0)) {
        sVar2 = -0x300;
        if ((*pbVar3 & 0xf) != 1) {
          sVar2 = 0x300;
        }
        sVar2 = *(short *)(pbVar3 + 3) + sVar2;
        *(short *)(pbVar3 + 3) = sVar2;
        if ((byte)((char)((ushort)sVar2 >> 8) + 9U) < 9) {
          *pbVar3 = *pbVar3 | 0x80;
        }
        FUN_ram_63a3(0xf8a7);
        if ((byte)((char)((ushort)sVar2 >> 8) + 0x1bU) < 10) {
          *pbVar3 = *pbVar3 | 0x80;
        }
        FUN_ram_67a0();
      }
      else {
        FUN_ram_634d();
      }
    }
    FUN_ram_63b1();
    pbVar3 = pbVar3 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:634d FUN_ram_634d -- automatic, unverified */

void FUN_ram_634d(undefined2 param_1,byte *param_2)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  
  cVar3 = (char)((ushort)param_1 >> 8);
  bVar1 = param_2[5] >> 1 == 3;
  if (bVar1) {
    param_2[7] = param_2[7] + 1;
    param_2[6] = param_2[7] >> 2 & 3;
    param_2[4] = DAT_ram_e604;
    FUN_ram_6409();
  }
  FUN_ram_63a3();
  if (bVar1) {
    bVar2 = (DAT_ram_e602 + '\b') - cVar3;
  }
  else {
    bVar2 = cVar3 + 0x1b;
  }
  if (bVar2 < 10) {
    *param_2 = *param_2 | 0x80;
  }
  FUN_ram_67a0();
  return;
}



/* ram:63a3 FUN_ram_63a3 -- automatic, unverified */

void FUN_ram_63a3(short param_1,short param_2)

{
  *(short *)(param_2 + 1) = *(short *)(param_2 + 1) + param_1;
  return;
}



/* ram:63b1 FUN_ram_63b1 -- automatic, unverified */

void FUN_ram_63b1(byte *param_1)

{
  byte bVar1;
  
  FUN_ram_405b((*param_1 & 0xf) * '\x04',0x3b10);
  func_0x0053();
  bVar1 = param_1[2];
  if (((*param_1 & 0x80) != 0) || (bVar1 == 0xd0)) {
    bVar1 = 0xc3;
  }
  *(byte *)(ushort)DAT_ram_0007 = bVar1;
  *(byte *)(ushort)DAT_ram_0007 = param_1[4];
  bVar1 = param_1[5];
  if (bVar1 == 5) {
    bVar1 = 0xc;
  }
  *(byte *)(ushort)DAT_ram_0007 = param_1[6] * '\x04' + (&DAT_ram_63fb)[bVar1 & 0xe];
  *(undefined *)(ushort)DAT_ram_0007 = (&DAT_ram_63fc)[bVar1 & 0xe];
  return;
}



/* ram:6409 FUN_ram_6409 -- automatic, unverified */

char FUN_ram_6409(byte *param_1)

{
  char cVar1;
  char *pcVar2;
  
  if ((*param_1 & 0x10) == 0) {
    pcVar2 = (char *)(*(short *)(param_1 + 8) + 1);
  }
  else {
    pcVar2 = (char *)(*(short *)(param_1 + 8) + -1);
  }
  if (*pcVar2 == -1) {
    pcVar2 = pcVar2 + -2;
    *param_1 = *param_1 | 0x10;
    cVar1 = '\0';
  }
  else {
    cVar1 = *pcVar2 + '\x02';
    if (cVar1 == '\0') {
      pcVar2 = pcVar2 + 1;
    }
  }
  param_1[8] = (byte)pcVar2;
  param_1[9] = (byte)((ushort)pcVar2 >> 8);
  return cVar1;
}



/* ram:6459 FUN_ram_6459 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x64e9) */
/* WARNING: Removing unreachable block (ram,0x64ef) */
/* WARNING: Removing unreachable block (ram,0x64da) */
/* WARNING: Removing unreachable block (ram,0x64f0) */

void FUN_ram_6459(void)

{
  char cVar1;
  byte *pbVar2;
  undefined1 *puVar3;
  
  if (1 < DAT_ram_e600) {
    return;
  }
  if (DAT_ram_e091 < 0xd8) {
    DAT_ram_e091 = DAT_ram_e091 + 1;
    DAT_ram_e09a = DAT_ram_e09a + 1;
    if ((2 < DAT_ram_e09a) && (DAT_ram_e09e = 1, 0x17 < DAT_ram_e09a)) {
      DAT_ram_e09a = 0;
      DAT_ram_e092 = DAT_ram_e092 + '\x01';
    }
    if (DAT_ram_e090 == '\0') {
      FUN_ram_55ed();
    }
    pbVar2 = &DAT_ram_e4f0;
    cVar1 = '\b';
    do {
      if ((*pbVar2 & 0x10) != 0) {
        pbVar2[2] = pbVar2[2] + 8;
      }
      pbVar2 = pbVar2 + 0x10;
      cVar1 = cVar1 + -1;
    } while (cVar1 != '\0');
    func_0x0053(0x3800);
    FUN_ram_64f9(DAT_ram_0007,(ushort)(DAT_ram_e091 & 3) * -0x20 + -0x1760);
    FUN_ram_64f9();
    FUN_ram_64f9();
    puVar3 = &DAT_ram_e600;
    FUN_ram_620c(DAT_ram_e610);
    FUN_ram_620c(puVar3[0x16]);
    return;
  }
  return;
}



/* ram:64a3 FUN_ram_64a3 -- automatic, unverified */

void FUN_ram_64a3(void)

{
  byte bVar1;
  undefined1 *puVar2;
  
  func_0x0053(0x3800);
  bVar1 = 0xe8a0 < (ushort)(DAT_ram_e091 & 3) * 0x20;
  FUN_ram_64f9(DAT_ram_e091 & 3,DAT_ram_0007,(ushort)(DAT_ram_e091 & 3) * -0x20 + -0x1760);
  FUN_ram_64f9();
  FUN_ram_64f9();
  puVar2 = &DAT_ram_e600;
  FUN_ram_620c(DAT_ram_e610);
  if ((bVar1 & 1) != 0) {
    FUN_ram_620c(puVar2[0x11]);
    if ((bool)(bVar1 & 1)) goto LAB_ram_64f0;
  }
  FUN_ram_620c(puVar2[0x16]);
  if ((bVar1 & 1) == 0) {
    return;
  }
  FUN_ram_620c(puVar2[0x17]);
  if ((bVar1 & 1) == 0) {
    return;
  }
LAB_ram_64f0:
  puVar2[2] = puVar2[2] + '\b';
  return;
}



/* ram:64f9 FUN_ram_64f9 -- automatic, unverified */

void FUN_ram_64f9(ushort param_1,undefined1 *param_2)

{
  char cVar1;
  
  cVar1 = (char)(param_1 >> 8);
  do {
    cVar1 = cVar1 + -1;
    *(undefined1 *)(param_1 & 0xff) = *param_2;
    param_2 = param_2 + 1;
  } while (cVar1 != '\0');
  return;
}



/* ram:64ff FUN_ram_64ff -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_64ff(void)

{
  undefined2 in_AF;
  byte bVar1;
  char cVar2;
  byte bVar3;
  char cVar4;
  undefined1 *puVar5;
  short sVar6;
  short sVar7;
  short sVar8;
  char *pcVar9;
  
  bVar1 = (byte)((ushort)in_AF >> 8);
  bVar3 = bVar1 >> 1;
  if (!(bool)(bVar1 & 1)) {
    puVar5 = _DAT_ram_e088;
    FUN_ram_405b();
    DAT_ram_e08a = *puVar5;
    pcVar9 = &DAT_ram_e08a;
    sVar6 = 0;
    for (; bVar3 != 0; bVar3 = bVar3 - 1) {
      sVar6 = sVar6 + 0x80;
    }
    sVar6 = sVar6 + -0x17e0;
    cVar2 = '\b';
    sVar8 = _DAT_ram_e09c;
    do {
      cVar4 = '\x04';
      sVar7 = sVar6;
      do {
        FUN_ram_6561(sVar7);
        sVar7 = sVar7 + 0x20;
        cVar4 = cVar4 + -1;
      } while (cVar4 != '\0');
      FUN_ram_405b(4);
      cVar4 = *pcVar9;
      *pcVar9 = cVar4 << 1;
      if (cVar4 < '\0') {
        sVar8 = sVar8 + 1;
      }
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    _DAT_ram_e09c = sVar8;
    return;
  }
  return;
}



/* ram:6502 FUN_ram_6502 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_6502(void)

{
  undefined2 in_AF;
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  char *pcVar7;
  
  puVar3 = _DAT_ram_e088;
  FUN_ram_405b();
  DAT_ram_e08a = *puVar3;
  pcVar7 = &DAT_ram_e08a;
  sVar4 = 0;
  for (cVar1 = (char)((ushort)in_AF >> 8); cVar1 != '\0'; cVar1 = cVar1 + -1) {
    sVar4 = sVar4 + 0x80;
  }
  sVar4 = sVar4 + -0x17e0;
  cVar1 = '\b';
  sVar6 = _DAT_ram_e09c;
  do {
    cVar2 = '\x04';
    sVar5 = sVar4;
    do {
      FUN_ram_6561(sVar5);
      sVar5 = sVar5 + 0x20;
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    FUN_ram_405b(4);
    cVar2 = *pcVar7;
    *pcVar7 = cVar2 << 1;
    if (cVar2 < '\0') {
      sVar6 = sVar6 + 1;
    }
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  _DAT_ram_e09c = sVar6;
  return;
}



/* ram:6529 FUN_ram_6529 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_6529(char param_1,ushort param_2,byte *param_3,short param_4)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  ushort uVar4;
  
  do {
    cVar3 = '\x04';
    uVar4 = param_2;
    do {
      FUN_ram_6561();
      bVar2 = 0xffdf < uVar4;
      uVar4 = uVar4 + 0x20;
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    FUN_ram_405b(4);
    bVar1 = *param_3;
    *param_3 = bVar1 << 1 | bVar2 & 1;
    if ((char)bVar1 < '\0') {
      param_4 = param_4 + 1;
    }
    param_1 = param_1 + -1;
  } while (param_1 != '\0');
  _DAT_ram_e09c = param_4;
  return;
}



/* ram:6561 FUN_ram_6561 -- automatic, unverified */

void FUN_ram_6561(undefined1 *param_1,undefined1 *param_2,byte *param_3)

{
  undefined1 uVar1;
  char cVar2;
  
  cVar2 = '\x04';
  if ((*param_3 & 0x80) != 0) {
    FUN_ram_405b(3);
    do {
      uVar1 = FUN_ram_6576();
      *param_1 = uVar1;
      param_1 = param_1 + -1;
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    return;
  }
  do {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *param_1 = uVar1;
    param_1 = param_1 + 1;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:6576 FUN_ram_6576 -- automatic, unverified */

byte FUN_ram_6576(byte *param_1)

{
  byte bVar1;
  
  bVar1 = *param_1;
  if ((*param_1 & 0xf0) == 0) {
    return bVar1;
  }
  if (bVar1 < 0xa0) {
    return bVar1 + 0x60;
  }
  return bVar1 + 0xa0;
}



/* ram:658d FUN_ram_658d -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_658d(void)

{
  char cVar1;
  undefined2 uVar2;
  short sVar3;
  undefined1 *puVar4;
  byte *pbVar5;
  short sVar6;
  undefined1 *puVar7;
  char *pcVar8;
  
  FUN_ram_66bf();
  if (DAT_ram_e290 == '\0') {
    _DAT_ram_e292 = CONCAT11(0xe0,DAT_ram_e292);
    if (DAT_ram_e3b0 != '\0') {
      return;
    }
    if (DAT_ram_e663 == '\0') {
      pbVar5 = &DAT_ram_65e6;
      FUN_ram_4079(DAT_ram_e062);
      while( true ) {
        if (*pbVar5 == 0xff) {
          return;
        }
        if ((*pbVar5 & 0x7f) == DAT_ram_e091 >> 1) break;
        pbVar5 = pbVar5 + 1;
      }
      FUN_ram_66a3();
      DAT_ram_e290 = 1;
      DAT_ram_e291 = *pbVar5 >> 7;
      DAT_ram_e29a = 0x28;
      if ((0x27 < DAT_ram_e604) && (DAT_ram_e29a = 0xd0, DAT_ram_e604 < 0xd0)) {
        DAT_ram_e29a = DAT_ram_e604;
      }
      return;
    }
    return;
  }
  if (DAT_ram_e600 < 2) {
    FUN_ram_6783();
    pcVar8 = &DAT_ram_6839;
    if (DAT_ram_e291 != '\0') {
      pcVar8 = &DAT_ram_6841;
    }
    FUN_ram_405b(DAT_ram_e29d,FUN_ram_66a3);
    cVar1 = *pcVar8;
    uVar2 = 0x200;
    if (cVar1 == '\0') {
      uVar2 = 0x1000;
    }
    FUN_ram_42ad(uVar2);
    if (cVar1 == '\0') {
      FUN_ram_490c(0x12);
      return;
    }
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0xa5);
  }
  if (DAT_ram_e290 == '\x02') {
    uVar2 = 0x803;
    if (DAT_ram_e291 != '\0') {
      uVar2 = 0x701;
    }
    DAT_ram_e29d = DAT_ram_e29d + 1;
    if ((byte)((ushort)uVar2 >> 8) <= DAT_ram_e29d) {
      DAT_ram_e29d = 0;
    }
    if (DAT_ram_e29c < (byte)uVar2) {
      DAT_ram_e29c = DAT_ram_e29d >> 2;
    }
  }
  DAT_ram_e290 = 1;
  FUN_ram_6848();
  sVar6 = _DAT_ram_e292 + _DAT_ram_e296;
  _DAT_ram_e292 = sVar6;
  sVar6 = _DAT_ram_e292;
  DAT_ram_e293 = (byte)((ushort)sVar6 >> 8);
  if (0xa7 < DAT_ram_e293) {
    if (DAT_ram_e293 < 0xd8) {
      puVar7 = &DAT_ram_66af;
      puVar4 = &DAT_ram_e290;
      sVar3 = 0x10;
      do {
        *puVar4 = *puVar7;
        puVar4 = puVar4 + 1;
        puVar7 = puVar7 + 1;
        sVar3 = sVar3 + -1;
      } while (sVar3 != 0);
      return;
    }
    _DAT_ram_e292 = sVar6;
    return;
  }
  _DAT_ram_e292 = sVar6;
  return;
}



/* ram:66a3 FUN_ram_66a3 -- automatic, unverified */

void FUN_ram_66a3(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  puVar3 = &DAT_ram_66af;
  puVar2 = &DAT_ram_e290;
  sVar1 = 0x10;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}



/* ram:66bf FUN_ram_66bf -- automatic, unverified */

void FUN_ram_66bf(short param_1)

{
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  char *pcVar4;
  undefined1 uVar5;
  undefined1 *puVar6;
  char *pcVar7;
  
  if (DAT_ram_e290 == '\0') {
    DAT_ram_e37c = 0xe0;
    DAT_ram_e378 = 0xe0;
    return;
  }
  DAT_ram_e37c = DAT_ram_e293;
  DAT_ram_e37d = DAT_ram_e295;
  DAT_ram_e37e = 0x94;
  puVar6 = &DAT_ram_e37f;
  DAT_ram_e37f = 1;
  if (DAT_ram_e291 != '\0') {
    puVar3 = &DAT_ram_674f;
    FUN_ram_4060(DAT_ram_e29d,param_1 + -2);
    *puVar6 = *puVar3;
  }
  pcVar4 = &DAT_ram_e378;
  if (DAT_ram_e291 == '\0') {
    cVar1 = DAT_ram_e295;
    cVar2 = DAT_ram_e293;
    FUN_ram_405b(DAT_ram_e29d);
    pcVar7 = &DAT_ram_6756;
    FUN_ram_4079();
    *pcVar4 = *pcVar7 + cVar2;
    uVar5 = (undefined1)((ushort)pcVar4 >> 8);
    *(char *)CONCAT11(uVar5,(char)pcVar4 + '\x01') = pcVar7[1] + cVar1;
    pcVar4 = (char *)CONCAT11(uVar5,(char)pcVar4 + '\x02');
    *pcVar4 = pcVar7[2];
    pcVar4 = pcVar4 + 1;
  }
  else {
    DAT_ram_e378 = DAT_ram_e293 + '\x03';
    DAT_ram_e379 = DAT_ram_e295 + '\x05';
    DAT_ram_e37a = 0x38;
    pcVar4 = &DAT_ram_e37b;
  }
  pcVar7 = &DAT_ram_674b;
  FUN_ram_4060(DAT_ram_e003 >> 2 & 3);
  *pcVar4 = *pcVar7;
  return;
}



/* ram:6783 FUN_ram_6783 -- automatic, unverified */

void FUN_ram_6783(void)

{
  DAT_ram_e0d0 = DAT_ram_e602;
  DAT_ram_e0d1 = DAT_ram_e604;
  FUN_ram_88d3(0x1010,CONCAT11(DAT_ram_e295,DAT_ram_e293),0xe0e);
  return;
}



/* ram:67a0 FUN_ram_67a0 -- automatic, unverified */

void FUN_ram_67a0(byte *param_1)

{
  byte bVar1;
  
  if (DAT_ram_e290 != '\x01') {
    return;
  }
  FUN_ram_88a4(0,(char)param_1 + '\x02');
  bVar1 = 0;
  FUN_ram_88d3(0x1010,CONCAT11(DAT_ram_e295,DAT_ram_e293));
  if ((bool)(bVar1 & 1)) {
    return;
  }
  FUN_ram_490c(0xe);
  *param_1 = *param_1 | 0x80;
  DAT_ram_e290 = 2;
  return;
}



/* ram:6848 FUN_ram_6848 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_6848(undefined2 param_1)

{
  short sVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  
  uVar3 = (undefined1)((ushort)param_1 >> 8);
  uVar2 = (undefined1)param_1;
  DAT_ram_e29b = DAT_ram_e29c + DAT_ram_e29b;
  FUN_ram_83e1();
  sVar1 = CONCAT11(uVar3,uVar2) * 0x20;
  _DAT_ram_e294 = CONCAT11(DAT_ram_e29a + (char)((ushort)sVar1 >> 8),(char)sVar1);
  return;
}



/* ram:6862 FUN_ram_6862 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_6862(undefined2 param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  byte *pbVar4;
  
  DAT_ram_e665 = (undefined1)((ushort)param_1 >> 8);
  bVar1 = *param_2;
  if ((bVar1 & 0x80) != 0) {
    bVar1 = bVar1 & 7;
    if (bVar1 < 4) {
      bVar2 = CARRY1(bVar1 * '\x02',bVar1 * '\x02');
      FUN_ram_4060(bVar1 * '\x04',0x6f24);
      bVar2 = bVar2 & 0xad;
      FUN_ram_6a04();
      bVar2 = bVar2 & 0xad;
      if ((*param_2 & 0x40) != 0) {
        return;
      }
      FUN_ram_6a67();
      if ((bool)(bVar2 & 1)) {
        return;
      }
      FUN_ram_490c(0x12);
    }
    *param_2 = *param_2 | 0x40;
                    /* WARNING: Subroutine does not return */
    inline_dispatch(*param_2 & 7);
  }
  if (bVar1 == 0) {
    if (DAT_ram_e091 != *(char *)(_DAT_ram_e570 + 1)) {
      return;
    }
    FUN_ram_6a1e();
    return;
  }
  if (param_2[1] != 0) {
    FUN_ram_6a04(param_2[1],0x6f40);
  }
  if (param_2[2] < 0xb0) {
    pbVar4 = &DAT_ram_e620;
    cVar3 = '\x03';
    do {
      if ((*pbVar4 & 0x80) == 0) {
        bVar1 = 0xfe < (byte)((char)pbVar4 + 1U);
        FUN_ram_88a4(CONCAT11((char)((ushort)pbVar4 >> 8),(char)pbVar4 + '\x02'));
        FUN_ram_88d3(0x808,*(undefined2 *)(param_2 + 2));
        if ((bVar1 & 1) == 0) {
          *pbVar4 = *pbVar4 | 0x80;
          FUN_ram_490c(0xf);
          param_2[1] = param_2[1] + 1;
          if (param_2[1] < param_2[8]) {
            return;
          }
          *param_2 = *param_2 | 0x80;
          FUN_ram_490c(0x10);
          return;
        }
      }
      pbVar4 = pbVar4 + 0x10;
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    return;
  }
  *param_2 = 0;
  param_2[1] = 0;
  return;
}



/* ram:6919 state_6919 -- automatic, unverified */

void state_6919(void)

{
  FUN_ram_42ad(0x500);
  return;
}



/* ram:691f state_691f -- automatic, unverified */

void state_691f(void)

{
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x56);
}



/* ram:6924 FUN_ram_6924 -- automatic, unverified */

void FUN_ram_6924(undefined2 param_1)

{
  char cVar1;
  byte *pbVar2;
  byte *pbVar3;
  
  pbVar3 = &DAT_ram_e100;
  cVar1 = '\a';
  do {
    pbVar2 = &DAT_ram_75aa;
    FUN_ram_405b(pbVar3[1] & 0xf);
    if ((*pbVar3 != 0) && (*pbVar3 < *pbVar2)) {
      FUN_ram_7579();
    }
    pbVar3 = pbVar3 + 0x20;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_ram_6974(param_1);
  return;
}



/* ram:694e state_694e -- automatic, unverified */

void state_694e(void)

{
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x3d);
}



/* ram:6974 FUN_ram_6974 -- automatic, unverified */

void FUN_ram_6974(void)

{
  char cVar1;
  undefined1 *puVar2;
  
  puVar2 = &DAT_ram_e200;
  cVar1 = '\b';
  do {
    *puVar2 = 0;
    FUN_ram_405b(0x10);
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:6983 state_6983 -- automatic, unverified */

void state_6983(void)

{
  FUN_ram_6a04(0x6f34);
  return;
}



/* ram:698a state_698a -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x69aa) */

void state_698a(short param_1)

{
  bool bVar1;
  bool bVar2;
  byte in_F;
  char cVar3;
  byte bVar4;
  char *pcVar5;
  
  FUN_ram_6a04(0x6f38);
  FUN_ram_6a67();
  if ((bool)(in_F & 1)) {
    return;
  }
  pcVar5 = &DAT_ram_6a8c;
  cVar3 = '\x06';
  do {
    if (*(char *)(param_1 + 9) == *pcVar5) {
      bVar4 = pcVar5[1];
      bVar1 = ((DAT_ram_e061 & 0xf) + (bVar4 & 0xf) & 0x10) != 0;
      bVar2 = CARRY1(DAT_ram_e061,bVar4);
      DAT_ram_e061 = BCDadjust(DAT_ram_e061 + bVar4,bVar2,bVar1);
      BCDadjustCarry(DAT_ram_e061,bVar2,bVar1);
      hasEvenParity(DAT_ram_e061);
      DAT_ram_e062 = DAT_ram_e062 + bVar4;
      DAT_ram_e00d = 1;
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0xb1);
    }
    pcVar5 = pcVar5 + 2;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return;
}



/* ram:69bc state_69bc -- automatic, unverified */

void state_69bc(undefined1 *param_1)

{
  if ((byte)(param_1[2] + 0x10) < 0xd0) {
    FUN_ram_8df1(0x404,CONCAT11(param_1[3],param_1[2]) + -0x810,0x9a9c);
    return;
  }
  *param_1 = 0;
  param_1[1] = 0;
  return;
}



/* ram:69d5 state_69d5 -- automatic, unverified */

void state_69d5(undefined1 *param_1)

{
  byte bVar1;
  byte bVar2;
  
  if (0xaf < (byte)param_1[2]) {
    *param_1 = 0;
    param_1[1] = 0;
    return;
  }
  bVar1 = param_1[8] & 0xf0;
  bVar2 = CARRY1(bVar1,bVar1);
  FUN_ram_6a71(bVar1 * '\x02',8);
  if (!(bool)(bVar2 & 1)) {
    if ((param_1[8] & 1) == 0) {
      if ((DAT_ram_e009 & 4) == 0) {
        return;
      }
    }
    else if ((DAT_ram_e009 & 8) == 0) {
      return;
    }
    DAT_ram_e604 = -0x10 - DAT_ram_e604;
    return;
  }
  return;
}



/* ram:6a04 FUN_ram_6a04 -- automatic, unverified */

void FUN_ram_6a04(undefined1 *param_1)

{
  if ((byte)param_1[2] < 0xb0) {
    FUN_ram_8df1(0x202);
    return;
  }
  *param_1 = 0;
  param_1[1] = 0;
  return;
}



/* ram:6a1e FUN_ram_6a1e -- automatic, unverified */

void FUN_ram_6a1e(byte *param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  byte *pbVar3;
  
  bVar2 = *param_1;
  if ((bVar2 != 0x15) || (DAT_ram_e061 <= DAT_ram_e006)) {
    bVar1 = bVar2 & 0x1f;
    if (0x16 < bVar1) {
      bVar1 = 0x97;
    }
    *param_2 = bVar1;
    param_2[2] = 0;
    param_2[3] = param_1[2];
    pbVar3 = &DAT_ram_6a7f;
    FUN_ram_405b(bVar2 & 0xf);
    bVar1 = *pbVar3;
    if ((bVar2 & 0x20) != 0) {
      param_2[1] = 1;
      bVar1 = bVar1 + 1;
    }
    param_2[8] = bVar1;
    param_2[9] = param_1[1];
  }
  return;
}



/* ram:6a67 FUN_ram_6a67 -- automatic, unverified */

void FUN_ram_6a67(short param_1)

{
  if (DAT_ram_e600 < 2) {
    FUN_ram_888f(0x808);
    FUN_ram_88d3(*(undefined1 *)(param_1 + 2));
    return;
  }
  return;
}



/* ram:6a71 FUN_ram_6a71 -- automatic, unverified */

void FUN_ram_6a71(short param_1)

{
  FUN_ram_888f();
  FUN_ram_88d3(*(undefined1 *)(param_1 + 2));
  return;
}



/* ram:6f44 FUN_ram_6f44 -- automatic, unverified */

void FUN_ram_6f44(void)

{
  char cVar1;
  
  if ((DAT_ram_e009 & 0x3f) != 0x2c) {
    return;
  }
  cVar1 = func_0x0141(3);
  if (cVar1 == -0x41) {
    FUN_ram_6f6b(1);
  }
  cVar1 = func_0x0141(4);
  if (cVar1 != -9) {
    cVar1 = func_0x0141(5);
    if (cVar1 != -0x41) {
      return;
    }
    DAT_ram_e069 = 0x10;
    return;
  }
  DAT_ram_e060 = 0x26;
  return;
}



/* ram:6f6b FUN_ram_6f6b -- automatic, unverified */

void FUN_ram_6f6b(undefined1 param_1)

{
  DAT_ram_e069 = param_1;
  return;
}



/* ram:6f75 FUN_ram_6f75 -- automatic, unverified */

byte FUN_ram_6f75(byte *param_1)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  
  if (DAT_ram_e09e == '\0') {
    DAT_ram_e1e1 = DAT_ram_e09e;
    return 0;
  }
  if ((DAT_ram_e003 & 0x1e) != 0) {
    return DAT_ram_e003 & 0x1e;
  }
  bVar2 = DAT_ram_e1e1 & 0x7f;
  if (bVar2 != 0) {
    DAT_ram_e1e1 = DAT_ram_e1e1 - 1;
    return bVar2;
  }
  if (!(bool)(DAT_ram_e1e1 >> 7)) {
    if (DAT_ram_e0a0 != 0) {
      return DAT_ram_e0a0;
    }
    if (DAT_ram_e0b0 != 0) {
      return DAT_ram_e0b0;
    }
    pbVar5 = &DAT_ram_e100;
    cVar3 = '\a';
    do {
      if (*pbVar5 != 0) {
        return *pbVar5;
      }
      FUN_ram_405b(0x20);
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
  }
  pbVar5 = &DAT_ram_e0a0;
  if (DAT_ram_e0a0 != 0) {
    pbVar5 = &DAT_ram_e0b0;
    if (DAT_ram_e0b0 != 0) {
      return DAT_ram_e0b0;
    }
  }
  cVar3 = '\b';
  pbVar6 = pbVar5;
  do {
    *pbVar6 = 0;
    pbVar6 = pbVar6 + 1;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  FUN_ram_4071(DAT_ram_e062,0x704b);
  pbVar6 = &DAT_ram_e09f;
  FUN_ram_4060(DAT_ram_e09f * '\x02');
  bVar1 = *param_1;
  *pbVar5 = bVar1;
  bVar2 = param_1[1] >> 4;
  if ((DAT_ram_e068 != '\0') && (bVar2 != 1)) {
    bVar2 = bVar2 + 2;
  }
  pbVar5[6] = bVar2;
  DAT_ram_e1e1 = (param_1[1] & 0xf) * '\x02';
  if ((bVar1 & 0x20) != 0) {
    DAT_ram_e1e1 = DAT_ram_e1e1 | 0x80;
  }
  if (*pbVar5 != 0) {
    pbVar5[8] = 1;
  }
  pbVar4 = (byte *)0x7043;
  FUN_ram_4060(DAT_ram_e062);
  bVar2 = *pbVar4;
  *pbVar6 = *pbVar6 + 1;
  if (bVar2 <= *pbVar6) {
    *pbVar6 = 0;
  }
  bVar2 = *pbVar5;
  if ((bVar2 & 0x40) == 0) {
    return bVar2;
  }
  pbVar6 = &DAT_ram_7033;
  FUN_ram_405b(bVar2 & 0xf);
  bVar2 = *pbVar6;
  pbVar5[3] = bVar2;
  return bVar2;
}



/* ram:7012 FUN_ram_7012 -- automatic, unverified */

void FUN_ram_7012(byte *param_1,byte *param_2,byte *param_3)

{
  byte bVar1;
  byte *pbVar2;
  
  *param_1 = (byte)param_2;
  FUN_ram_4060();
  bVar1 = *param_2;
  *param_1 = *param_1 + 1;
  if (bVar1 <= *param_1) {
    *param_1 = 0;
  }
  if ((*param_3 & 0x40) == 0) {
    return;
  }
  pbVar2 = &DAT_ram_7033;
  FUN_ram_405b(*param_3 & 0xf);
  param_3[3] = *pbVar2;
  return;
}



/* ram:729b FUN_ram_729b -- automatic, unverified */

void FUN_ram_729b(void)

{
  char cVar1;
  undefined1 *puVar2;
  
  if (199 < DAT_ram_e091) {
    return;
  }
  puVar2 = &DAT_ram_e0a0;
  cVar1 = '\x02';
  do {
    FUN_ram_72b4(puVar2);
    puVar2 = puVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:72b4 FUN_ram_72b4 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x72fe) */
/* WARNING: Removing unreachable block (ram,0x7316) */
/* WARNING: Removing unreachable block (ram,0x7342) */
/* WARNING: Removing unreachable block (ram,0x7344) */
/* WARNING: Removing unreachable block (ram,0x7349) */
/* WARNING: Removing unreachable block (ram,0x734f) */
/* WARNING: Removing unreachable block (ram,0x736f) */
/* WARNING: Removing unreachable block (ram,0x7379) */
/* WARNING: Removing unreachable block (ram,0x737c) */
/* WARNING: Removing unreachable block (ram,0x7382) */
/* WARNING: Removing unreachable block (ram,0x7384) */
/* WARNING: Removing unreachable block (ram,0x739a) */
/* WARNING: Removing unreachable block (ram,0x739b) */
/* WARNING: Removing unreachable block (ram,0x73a2) */
/* WARNING: Removing unreachable block (ram,0x73b9) */
/* WARNING: Removing unreachable block (ram,0x73c2) */
/* WARNING: Removing unreachable block (ram,0x73c3) */

void FUN_ram_72b4(byte *param_1)

{
  byte *pbVar1;
  
  if (*param_1 == 0) {
    return;
  }
  if (param_1[8] != 0) {
    return;
  }
  if (param_1[5] == 0) {
    if (param_1[1] != 0) {
      param_1[1] = param_1[1] - 1;
      return;
    }
    if ((*param_1 & 0x40) != 0) {
      param_1[5] = param_1[3];
      return;
    }
  }
  else {
    if (param_1[4] != 0) {
      param_1[4] = param_1[4] - 1;
      return;
    }
    pbVar1 = &DAT_ram_73ce;
    FUN_ram_405b(*param_1 & 0xf);
    param_1[4] = *pbVar1;
    param_1[5] = param_1[5] - 1;
  }
  FUN_ram_73de();
  return;
}



/* ram:73de FUN_ram_73de -- automatic, unverified */

byte FUN_ram_73de(void)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_e100;
  cVar2 = '\a';
  do {
    if (*pcVar3 == '\0') {
      return 0;
    }
    bVar1 = FUN_ram_405b(0x20);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return bVar1 | 1;
}



/* ram:741d FUN_ram_741d -- automatic, unverified */

char FUN_ram_741d(void)

{
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  
  if (DAT_ram_e3b0 != '\0') {
    return DAT_ram_e3b0;
  }
  if (0xc9 < DAT_ram_e091) {
    FUN_ram_6924();
  }
  FUN_ram_772a();
  if (DAT_ram_e663 == '\0') {
    FUN_ram_6f75(0);
    FUN_ram_729b();
  }
  puVar3 = &DAT_ram_e100;
  cVar2 = '\a';
  do {
    FUN_ram_75da(puVar3);
    FUN_ram_749a();
    cVar1 = FUN_ram_7452();
    puVar3 = puVar3 + 0x20;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return cVar1;
}



/* ram:7452 FUN_ram_7452 -- automatic, unverified */

void FUN_ram_7452(byte *param_1,undefined2 param_2,byte *param_3)

{
  byte in_F;
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  
  bVar1 = FUN_ram_748a();
  if ((bool)(in_F >> 6 & 1)) {
    return;
  }
  if (*param_1 <= bVar1) {
    return;
  }
  bVar1 = (param_3[1] & 0xf) < 7;
  if ((param_3[1] & 0xf) == 7) {
    bVar1 = *param_3 < 4;
    if (!(bool)bVar1) {
      return;
    }
  }
  FUN_ram_8755(&DAT_ram_e600);
  if ((bool)(bVar1 & 1)) {
    return;
  }
  if (1 < DAT_ram_e600) {
    return;
  }
  if (DAT_ram_e60c < 2) {
    FUN_ram_8338();
  }
  else {
    if (DAT_ram_e60c == 2) {
      return;
    }
    cVar2 = '\0';
    if (DAT_ram_e663 != '\0') {
      cVar2 = '\x10';
    }
    pbVar3 = &DAT_ram_75ba;
    FUN_ram_405b((param_3[1] & 0xf) + cVar2);
    FUN_ram_42ad(param_2,(ushort)*pbVar3 * 0x10);
  }
  pbVar3 = &DAT_ram_75aa;
  FUN_ram_405b(param_3[1] & 0xf);
  *param_3 = *pbVar3;
  param_3[0xe] = 6;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x4d);
}



/* ram:748a FUN_ram_748a -- automatic, unverified */

undefined1 FUN_ram_748a(undefined1 *param_1)

{
  FUN_ram_405b(param_1[1] & 0xf,&DAT_ram_75aa);
  return *param_1;
}



/* ram:749a FUN_ram_749a -- automatic, unverified */

void FUN_ram_749a(byte *param_1,undefined2 param_2,byte *param_3)

{
  byte bVar1;
  byte in_F;
  bool bVar2;
  byte bVar3;
  byte bVar4;
  char cVar5;
  byte *pbVar6;
  
  bVar4 = FUN_ram_748a();
  if ((bool)(in_F >> 6 & 1)) {
    return;
  }
  if (*param_1 <= bVar4) {
    return;
  }
  bVar4 = param_3[1] & 0xf;
  if (bVar4 == 0) {
    bVar2 = false;
    if (DAT_ram_e663 == 0) {
      if (*param_3 < 3) {
        return;
      }
      bVar2 = *param_3 < 5;
      if (!bVar2) {
        return;
      }
    }
  }
  else if ((bVar4 == 0xf) && (bVar4 = *param_3, 2 < bVar4)) {
    if (bVar4 != 5) {
      return;
    }
    bVar2 = false;
  }
  else if (bVar4 == 7) {
    bVar2 = *param_3 < 4;
    if (!bVar2) {
      return;
    }
  }
  else {
    bVar2 = bVar4 < 0xc;
    if (((bVar4 == 0xc) && (bVar2 = false, DAT_ram_e663 == 0)) && (bVar2 = *param_3 < 3, !bVar2)) {
      return;
    }
  }
  bVar4 = DAT_ram_e620 >> 7;
  if (!(bool)bVar4) {
    FUN_ram_8767(DAT_ram_e620 << 1 | bVar2);
    pbVar6 = &DAT_ram_e625;
    if ((bVar4 & 1) == 0) goto LAB_ram_750f;
  }
  bVar3 = DAT_ram_e630 >> 7;
  if (!(bool)bVar3) {
    FUN_ram_8767(DAT_ram_e630 << 1 | bVar4 & 1);
    pbVar6 = &DAT_ram_e635;
    if ((bVar3 & 1) == 0) goto LAB_ram_750f;
  }
  bVar4 = DAT_ram_e640 >> 7;
  if ((bool)bVar4) {
    return;
  }
  FUN_ram_8767(DAT_ram_e640 << 1 | bVar3 & 1);
  pbVar6 = &DAT_ram_e645;
  if ((bool)(bVar4 & 1)) {
    return;
  }
LAB_ram_750f:
  bVar4 = *pbVar6;
  if (bVar4 >> 1 < 3) {
    pbVar6 = (byte *)CONCAT11((char)((ushort)pbVar6 >> 8),(char)pbVar6 + -5);
    *pbVar6 = *pbVar6 | 0x80;
  }
  bVar3 = param_3[1] & 0xf;
  if ((((param_3[1] & 0xf) == 0xf) && (bVar3 = *param_3, *param_3 < 3)) &&
     (bVar3 = DAT_ram_e663, DAT_ram_e663 == 0)) {
    *param_3 = 3;
    FUN_ram_490c(10);
    return;
  }
  if (((bVar3 == 7) && (bVar1 = param_3[0x14] - 1, param_3[0x14] = bVar1, bVar1 != 0)) &&
     (bVar3 = DAT_ram_e663, DAT_ram_e663 == 0)) {
    *param_3 = 4;
    FUN_ram_490c(10);
    return;
  }
  if ((bVar3 == 0xc) && (DAT_ram_e663 == 0)) {
    *param_3 = 3;
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0x4d);
  }
  if ((bVar4 >> 1 < 3) && (bVar4 = param_3[0xb] - 1, param_3[0xb] = bVar4, bVar4 != 0)) {
    return;
  }
  cVar5 = '\0';
  if (DAT_ram_e663 != 0) {
    cVar5 = '\x10';
  }
  pbVar6 = &DAT_ram_75ba;
  FUN_ram_405b((param_3[1] & 0xf) + cVar5);
  FUN_ram_42ad(param_2,(ushort)*pbVar6 * 0x10);
  pbVar6 = &DAT_ram_75aa;
  FUN_ram_405b(param_3[1] & 0xf);
  *param_3 = *pbVar6;
  param_3[0xe] = 6;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x4d);
}



/* ram:7579 FUN_ram_7579 -- automatic, unverified */

void FUN_ram_7579(undefined2 param_1,undefined1 *param_2)

{
  byte *pbVar1;
  undefined1 *puVar2;
  
  pbVar1 = &DAT_ram_75ba;
  FUN_ram_405b((param_2[1] & 0xf) + 0x10);
  FUN_ram_42ad(param_1,(ushort)*pbVar1 * 0x10);
  puVar2 = &DAT_ram_75aa;
  FUN_ram_405b(param_2[1] & 0xf);
  *param_2 = *puVar2;
  param_2[0xe] = 6;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x4d);
}



/* ram:75da FUN_ram_75da -- automatic, unverified */

void FUN_ram_75da(byte *param_1,short param_2)

{
  byte bVar1;
  
  if (DAT_ram_e663 != '\0') {
    bVar1 = FUN_ram_748a(DAT_ram_e663);
    if (bVar1 < *param_1) {
      return;
    }
  }
  FUN_ram_4079(*(byte *)(param_2 + 1) & 0xf,0x75f3);
  FUN_ram_4065();
  return;
}



/* ram:75ea FUN_ram_75ea -- automatic, unverified */

void FUN_ram_75ea(void)

{
  FUN_ram_4079(0x75f3);
  FUN_ram_4065();
  return;
}



/* ram:7613 FUN_ram_7613 -- automatic, unverified */

void FUN_ram_7613(void)

{
  char cVar1;
  undefined1 *puVar2;
  
  puVar2 = &DAT_ram_e103;
  cVar1 = '\a';
  do {
    *puVar2 = 0xe0;
    FUN_ram_405b(0x20);
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:7622 FUN_ram_7622 -- automatic, unverified */

byte FUN_ram_7622(void)

{
  byte in_F;
  byte bVar1;
  byte bVar2;
  char cVar3;
  char *pcVar4;
  
  cVar3 = '\x04';
  pcVar4 = &DAT_ram_e620;
  do {
    bVar1 = -(*pcVar4 >> 7);
    if (bVar1 == 0) {
      return *pcVar4 << 1 | in_F & 1;
    }
    bVar2 = FUN_ram_405b(0x10);
    cVar3 = cVar3 + -1;
    in_F = bVar1;
  } while (cVar3 != '\0');
  return bVar2;
}



/* ram:7633 FUN_ram_7633 -- automatic, unverified */

byte FUN_ram_7633(short param_1)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  char *pcVar4;
  byte *pbVar5;
  
  if (0xbf < *(byte *)(param_1 + 3)) {
    return *(byte *)(param_1 + 3);
  }
  bVar2 = *(byte *)(param_1 + 1) & 0xf;
  pcVar4 = &DAT_ram_76ed;
  FUN_ram_405b(bVar2);
  cVar3 = *pcVar4;
  if ((DAT_ram_e061 - 1U & 0xf) < 3) {
    cVar3 = cVar3 + -1;
  }
  pbVar5 = &DAT_ram_76cd;
  FUN_ram_4079(bVar2);
  if (((DAT_ram_e061 - 1U & 0xf) < 3) && (*(char *)(param_1 + 0xf) == -1)) {
    return 0xff;
  }
  bVar2 = *(char *)(param_1 + 0xf) + 1;
  *(byte *)(param_1 + 0xf) = bVar2;
  while (bVar2 != *pbVar5) {
    pbVar5 = pbVar5 + 1;
    cVar3 = cVar3 + -1;
    if (cVar3 == '\0') {
      return bVar2;
    }
  }
  bVar2 = (DAT_ram_e602 + 8U) - *(byte *)(param_1 + 3);
  if ((byte)(DAT_ram_e602 + 8U) < *(byte *)(param_1 + 3)) {
    bVar2 = -bVar2;
  }
  if (bVar2 < 0x18) {
    return bVar2;
  }
  bVar2 = (DAT_ram_e604 + 8U) - *(byte *)(param_1 + 5);
  if ((byte)(DAT_ram_e604 + 8U) < *(byte *)(param_1 + 5)) {
    bVar2 = -bVar2;
  }
  if (bVar2 < 0x18) {
    return bVar2;
  }
  bVar2 = *(byte *)(param_1 + 1);
  if ((DAT_ram_e068 == '\0') && (DAT_ram_e062 < 4)) {
    if (-1 < (char)bVar2) {
      return bVar2 << 1 | 1;
    }
    if ((*(byte *)(param_1 + 0x11) != 0xff) && (bVar1 = *(byte *)(param_1 + 0x11) & 1, bVar1 != 0))
    {
      return bVar1;
    }
  }
  FUN_ram_405b(bVar2 & 0xf);
  bVar2 = FUN_ram_857f(4);
  return bVar2;
}



/* ram:772a FUN_ram_772a -- automatic, unverified */

void FUN_ram_772a(void)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char *pcVar4;
  char cVar5;
  char *pcVar6;
  undefined2 *puVar7;
  undefined1 uVar8;
  undefined1 *puVar9;
  
  puVar9 = &DAT_ram_e100;
  pcVar6 = &DAT_ram_e320;
  cVar3 = '\a';
  do {
    puVar7 = (undefined2 *)&DAT_ram_7772;
    FUN_ram_405b(puVar9[10] * '\x02');
    pcVar4 = (char *)*puVar7;
    cVar2 = '\x02';
    do {
      *pcVar6 = *pcVar4 + puVar9[3];
      cVar5 = (char)pcVar6;
      uVar8 = (undefined1)((ushort)pcVar6 >> 8);
      *(char *)CONCAT11(uVar8,cVar5 + '\x01') = pcVar4[1] + puVar9[5];
      cVar1 = pcVar4[2];
      if (4 < (byte)puVar9[10]) {
        cVar1 = cVar1 + puVar9[0x16];
      }
      *(char *)CONCAT11(uVar8,cVar5 + '\x02') = cVar1;
      *(char *)CONCAT11(uVar8,cVar5 + '\x03') = pcVar4[3];
      pcVar4 = pcVar4 + 4;
      pcVar6 = (char *)CONCAT11(uVar8,cVar5 + '\x04');
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    puVar9 = puVar9 + 0x20;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return;
}



/* ram:78e6 FUN_ram_78e6 -- automatic, unverified */

void FUN_ram_78e6(void)

{
  char cVar1;
  undefined1 *puVar2;
  
  puVar2 = &DAT_ram_e0a0;
  cVar1 = '\x02';
  do {
    FUN_ram_78f9(puVar2);
    puVar2 = puVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:78f8 FUN_ram_78f8 -- automatic, unverified */

void FUN_ram_78f8(void)

{
  return;
}



/* ram:78f9 FUN_ram_78f9 -- automatic, unverified */

byte FUN_ram_78f9(undefined2 param_1,byte *param_2)

{
  byte *pbVar1;
  undefined2 uVar2;
  
  if (param_2[8] == 0) {
    return 0;
  }
  param_2[8] = 0;
  pbVar1 = &DAT_ram_7950;
  FUN_ram_405b(DAT_ram_e1f1);
  param_2[7] = *pbVar1;
  FUN_ram_4071(*param_2 & 0xf,0xa993);
  uVar2 = 0x7944;
  FUN_ram_4079(DAT_ram_e1f1);
  FUN_ram_4412(param_1);
  FUN_ram_4071(DAT_ram_e1f1,0x794a);
  FUN_ram_4478(4,uVar2);
  DAT_ram_e1f1 = DAT_ram_e1f1 + 1;
  if (2 < DAT_ram_e1f1) {
    DAT_ram_e1f1 = 0;
  }
  return DAT_ram_e1f1;
}



/* ram:79a5 FUN_ram_79a5 -- automatic, unverified */

void FUN_ram_79a5(undefined2 param_1)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  undefined1 uVar6;
  char *pcVar4;
  byte *pbVar5;
  
  FUN_ram_7633();
  cVar3 = (char)param_1;
  uVar6 = (undefined1)((ushort)param_1 >> 8);
  pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x02');
  *pcVar4 = *(char *)CONCAT11(uVar6,cVar3 + 6U) + *pcVar4;
  pbVar5 = (byte *)CONCAT11(uVar6,cVar3 + '\x03');
  bVar1 = *(char *)CONCAT11(uVar6,cVar3 + '\a') + *pbVar5 + (0xfe < (byte)(cVar3 + 6U));
  *pbVar5 = bVar1;
  if ((bVar1 < 0xa8) || (0xe7 < bVar1)) {
    pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x04');
    *pcVar4 = *(char *)CONCAT11(uVar6,cVar3 + 8U) + *pcVar4;
    pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x05');
    cVar2 = *(char *)CONCAT11(uVar6,cVar3 + '\t') + *pcVar4 + (0xfe < (byte)(cVar3 + 8U));
    *pcVar4 = cVar2;
    if ((byte)(cVar2 - 8U) < 0xf0) {
      return;
    }
    pbVar5 = (byte *)CONCAT11(uVar6,cVar3 + '\x03');
  }
  *pbVar5 = 0xe0;
  *(undefined1 *)CONCAT11((char)((ushort)pbVar5 >> 8),(char)pbVar5 + -3) = 0;
  return;
}



/* ram:79a8 FUN_ram_79a8 -- automatic, unverified */

void FUN_ram_79a8(undefined2 param_1)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  undefined1 uVar6;
  char *pcVar4;
  byte *pbVar5;
  
  cVar3 = (char)param_1;
  uVar6 = (undefined1)((ushort)param_1 >> 8);
  pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x02');
  *pcVar4 = *(char *)CONCAT11(uVar6,cVar3 + 6U) + *pcVar4;
  pbVar5 = (byte *)CONCAT11(uVar6,cVar3 + '\x03');
  bVar1 = *(char *)CONCAT11(uVar6,cVar3 + '\a') + *pbVar5 + (0xfe < (byte)(cVar3 + 6U));
  *pbVar5 = bVar1;
  if ((bVar1 < 0xa8) || (0xe7 < bVar1)) {
    pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x04');
    *pcVar4 = *(char *)CONCAT11(uVar6,cVar3 + 8U) + *pcVar4;
    pcVar4 = (char *)CONCAT11(uVar6,cVar3 + '\x05');
    cVar2 = *(char *)CONCAT11(uVar6,cVar3 + '\t') + *pcVar4 + (0xfe < (byte)(cVar3 + 8U));
    *pcVar4 = cVar2;
    if ((byte)(cVar2 - 8U) < 0xf0) {
      return;
    }
    pbVar5 = (byte *)CONCAT11(uVar6,cVar3 + '\x03');
  }
  *pbVar5 = 0xe0;
  *(undefined1 *)CONCAT11((char)((ushort)pbVar5 >> 8),(char)pbVar5 + -3) = 0;
  return;
}



/* ram:79fa FUN_ram_79fa -- automatic, unverified */

void FUN_ram_79fa(undefined2 param_1,short param_2)

{
  char cVar1;
  
  cVar1 = (char)((ushort)param_1 >> 8);
  if ((DAT_ram_e003 & 8) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  *(char *)(param_2 + 10) = cVar1;
  return;
}



/* ram:7a5e FUN_ram_7a5e -- automatic, unverified */

void FUN_ram_7a5e(undefined1 *param_1)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  puVar3 = param_1;
  FUN_ram_405b(6);
  sVar1 = 9;
  *param_1 = 0;
  puVar2 = param_1;
  do {
    puVar2 = puVar2 + 1;
    *puVar2 = *param_1;
    param_1 = param_1 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  puVar3[0xb] = 1;
  return;
}



/* ram:7a84 FUN_ram_7a84 -- automatic, unverified */

void FUN_ram_7a84(short param_1)

{
  byte bVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  *(undefined1 *)(param_1 + 7) = 2;
  *(undefined1 *)(param_1 + 10) = 9;
  bVar1 = DAT_ram_e604;
  if ((*(byte *)(param_1 + 0x11) != 0xff) &&
     (bVar1 = DAT_ram_e1f0, (*(byte *)(param_1 + 0x11) & 0xf) == 0)) {
    DAT_ram_e1f0 = DAT_ram_e604;
    bVar1 = DAT_ram_e604;
  }
  uVar2 = 0x380;
  uVar3 = 0x2010;
  if (bVar1 < 0x80) {
    uVar2 = 0xfc80;
    uVar3 = 0xe0f0;
  }
  *(char *)(param_1 + 8) = (char)uVar2;
  *(char *)(param_1 + 9) = (char)((ushort)uVar2 >> 8);
  *(char *)(param_1 + 0xd) = (char)((ushort)uVar3 >> 8);
  *(char *)(param_1 + 5) = (char)uVar3;
  FUN_ram_7afe();
  return;
}



/* ram:7afe FUN_ram_7afe -- automatic, unverified */

char FUN_ram_7afe(char *param_1)

{
  char cVar1;
  
  cVar1 = *param_1;
  if (*param_1 == '\0') {
    return cVar1;
  }
  *param_1 = *param_1 + '\x01';
  return cVar1;
}



/* ram:7b9a FUN_ram_7b9a -- automatic, unverified */

char FUN_ram_7b9a(char *param_1)

{
  char cVar1;
  
  cVar1 = *param_1;
  if (*param_1 == '\0') {
    return cVar1;
  }
  *param_1 = *param_1 + -1;
  return cVar1;
}



/* ram:7bf9 FUN_ram_7bf9 -- automatic, unverified */

void FUN_ram_7bf9(undefined1 param_1,short param_2)

{
  *(undefined1 *)(param_2 + 0xe) = param_1;
  FUN_ram_7afe();
  return;
}



/* ram:7c74 state_7c74 -- automatic, unverified */

void state_7c74(short param_1)

{
  char cVar1;
  undefined1 uVar2;
  
  uVar2 = 1;
  if ((DAT_ram_e003 & 2) != 0) {
    uVar2 = 2;
  }
  *(undefined1 *)(param_1 + 10) = uVar2;
  cVar1 = *(char *)(param_1 + 0xe) + -1;
  *(char *)(param_1 + 0xe) = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  FUN_ram_7bf9(6);
  return;
}



/* ram:7c8a state_7c8a -- automatic, unverified */

void state_7c8a(short param_1)

{
  char cVar1;
  undefined1 uVar2;
  
  uVar2 = 2;
  if ((DAT_ram_e003 & 2) != 0) {
    uVar2 = 3;
  }
  *(undefined1 *)(param_1 + 10) = uVar2;
  cVar1 = *(char *)(param_1 + 0xe) + -1;
  *(char *)(param_1 + 0xe) = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  FUN_ram_7bf9(6);
  return;
}



/* ram:7ca0 state_7ca0 -- automatic, unverified */

void state_7ca0(undefined1 *param_1)

{
  char cVar1;
  undefined1 uVar2;
  char *pcVar3;
  char *pcVar4;
  
  uVar2 = 3;
  if ((DAT_ram_e003 & 2) != 0) {
    uVar2 = 0;
  }
  param_1[10] = uVar2;
  cVar1 = param_1[0xe] + -1;
  param_1[0xe] = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  if ((DAT_ram_e600 != '\x02') && ((param_1[1] & 0x40) != 0)) {
    pcVar3 = &DAT_ram_e1e4;
    FUN_ram_405b(((byte)param_1[0x11] >> 4) * '\x03');
    pcVar4 = pcVar3 + 1;
    *pcVar4 = *pcVar4 + '\x01';
    if (*pcVar3 == *pcVar4) {
      FUN_ram_42ad(0x1000);
      param_1[10] = 4;
      FUN_ram_7bf9(8);
      return;
    }
  }
  *param_1 = 0;
  param_1[3] = 0xe0;
  return;
}



/* ram:7ce6 FUN_ram_7ce6 -- automatic, unverified */

void FUN_ram_7ce6(undefined1 *param_1)

{
  *param_1 = 0;
  param_1[3] = 0xe0;
  return;
}



/* ram:7cef state_7cef -- automatic, unverified */

void state_7cef(short param_1)

{
  char cVar1;
  
  cVar1 = *(char *)(param_1 + 0xe) + -1;
  *(char *)(param_1 + 0xe) = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  FUN_ram_7ce6();
  return;
}



/* ram:7cfc FUN_ram_7cfc -- automatic, unverified */

void FUN_ram_7cfc(undefined1 *param_1,short param_2)

{
  undefined1 uVar1;
  
  uVar1 = (undefined1)((ushort)param_1 >> 8);
  *param_1 = uVar1;
  *param_1 = uVar1;
  func_0x0028(uVar1);
  FUN_ram_7a5e((char)((ushort)param_1 >> 8));
  *(undefined1 *)(param_2 + 7) = 3;
  *(undefined1 *)(param_2 + 10) = 0xd;
  uVar1 = 0x20;
  if (0x7f < *(byte *)(param_2 + 5)) {
    uVar1 = 0xe0;
  }
  *(undefined1 *)(param_2 + 5) = uVar1;
  FUN_ram_7afe();
  return;
}



/* ram:7d87 FUN_ram_7d87 -- automatic, unverified */

void FUN_ram_7d87(void)

{
  FUN_ram_79fa();
  FUN_ram_79a5();
  return;
}



/* ram:7e3c thunk_FUN_ram_7e41 -- automatic, unverified */

void thunk_FUN_ram_7e41(short param_1)

{
  *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) ^ 1;
  FUN_ram_7b9a();
  return;
}



/* ram:7e41 FUN_ram_7e41 -- automatic, unverified */

void FUN_ram_7e41(short param_1)

{
  *(byte *)(param_1 + 0x10) = *(byte *)(param_1 + 0x10) ^ 1;
  FUN_ram_7b9a();
  return;
}



/* ram:7eb9 state_7eb9 -- automatic, unverified */

void state_7eb9(short param_1)

{
  FUN_ram_7a5e();
  *(undefined1 *)(param_1 + 7) = 1;
  *(undefined1 *)(param_1 + 10) = 0x18;
  *(undefined1 *)(param_1 + 0xb) = 3;
  FUN_ram_7afe();
  return;
}



/* ram:7ebc state_7ebc -- automatic, unverified */

void state_7ebc(short param_1)

{
  *(undefined1 *)(param_1 + 7) = 1;
  *(undefined1 *)(param_1 + 10) = 0x18;
  *(undefined1 *)(param_1 + 0xb) = 3;
  FUN_ram_7afe();
  return;
}



/* ram:7edd state_7edd -- automatic, unverified */

void state_7edd(byte *param_1,short param_2)

{
  undefined2 uVar1;
  
  uVar1 = 0x120;
  if (0x7f < *param_1) {
    uVar1 = 0xfee0;
  }
  *(char *)(param_2 + 8) = (char)uVar1;
  *(char *)(param_2 + 9) = (char)((ushort)uVar1 >> 8);
  *(undefined1 *)(param_2 + 6) = 0;
  *(undefined1 *)(param_2 + 7) = 0;
  *(undefined1 *)(param_2 + 0xe) = 0x30;
  FUN_ram_7afe();
  return;
}



/* ram:7ee0 state_7ee0 -- automatic, unverified */

void state_7ee0(byte param_1,undefined2 param_2,short param_3)

{
  undefined2 uVar1;
  
  uVar1 = 0x120;
  if (!CARRY1(param_1,(byte)((ushort)param_2 >> 8))) {
    uVar1 = 0xfee0;
  }
  *(char *)(param_3 + 8) = (char)uVar1;
  *(char *)(param_3 + 9) = (char)((ushort)uVar1 >> 8);
  *(undefined1 *)(param_3 + 6) = 0;
  *(undefined1 *)(param_3 + 7) = 0;
  *(undefined1 *)(param_3 + 0xe) = 0x30;
  FUN_ram_7afe();
  return;
}



/* ram:7f15 state_7f15 -- automatic, unverified */

void state_7f15(void)

{
  FUN_ram_7633();
  FUN_ram_8096();
  return;
}



/* ram:7f18 FUN_ram_7f18 -- automatic, unverified */

void FUN_ram_7f18(void)

{
  FUN_ram_8096();
  return;
}



/* ram:8012 FUN_ram_8012 -- automatic, unverified */

void FUN_ram_8012(undefined2 param_1,short param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  
  uVar1 = (undefined1)((ushort)param_1 >> 8);
  func_0x0028();
  FUN_ram_7a5e(uVar1);
  *(undefined1 *)(param_2 + 6) = 0x80;
  *(undefined1 *)(param_2 + 10) = 0x1c;
  *(undefined1 *)(param_2 + 0xc) = 0x10;
  *(undefined1 *)(param_2 + 0x12) = 0;
  uVar1 = 0x10;
  uVar2 = 1;
  if (0x7f < *(byte *)(param_2 + 5)) {
    uVar1 = 0xf0;
    uVar2 = 0xff;
  }
  *(undefined1 *)(param_2 + 5) = uVar1;
  *(undefined1 *)(param_2 + 8) = 0;
  *(undefined1 *)(param_2 + 9) = uVar2;
  FUN_ram_7afe();
  return;
}



/* ram:8081 FUN_ram_8081 -- automatic, unverified */

void FUN_ram_8081(undefined2 param_1,short param_2,short param_3)

{
  *(char *)(param_3 + 6) = (char)param_1;
  *(char *)(param_3 + 7) = (char)((ushort)param_1 >> 8);
  *(short *)(param_3 + 8) = param_2 * 5;
  FUN_ram_7afe();
  return;
}



/* ram:8096 FUN_ram_8096 -- automatic, unverified */

void FUN_ram_8096(void)

{
  FUN_ram_79fa();
  FUN_ram_79a8();
  return;
}



/* ram:80f8 FUN_ram_80f8 -- automatic, unverified */

byte FUN_ram_80f8(void)

{
  byte bVar1;
  
  FUN_ram_7d87();
  if ((DAT_ram_e003 & 0x3e) != 0) {
    return DAT_ram_e003 & 0x3e;
  }
  bVar1 = FUN_ram_490c(8);
  return bVar1;
}



/* ram:8111 FUN_ram_8111 -- automatic, unverified */

void FUN_ram_8111(undefined2 param_1,short param_2)

{
  undefined1 uVar1;
  
  uVar1 = (undefined1)((ushort)param_1 >> 8);
  func_0x0028();
  FUN_ram_7a5e(uVar1);
  *(undefined1 *)(param_2 + 6) = 0x80;
  *(undefined1 *)(param_2 + 10) = 0x20;
  FUN_ram_7afe();
  return;
}



/* ram:81ba FUN_ram_81ba -- automatic, unverified */

byte FUN_ram_81ba(void)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  byte bVar4;
  undefined1 uVar7;
  char *pcVar5;
  byte *pbVar6;
  undefined1 *puVar8;
  undefined1 *puVar9;
  
  if (DAT_ram_e3b0 != 0) {
    if (DAT_ram_e065 == '\x01') {
      return 1;
    }
    if (DAT_ram_e065 == '\x03') {
      return 3;
    }
  }
  DAT_ram_e0c1 = 0;
  puVar9 = &DAT_ram_e200;
  if (DAT_ram_e200 == '\x01') {
    FUN_ram_8278(0x81e7);
    FUN_ram_830f();
    cVar3 = (char)puVar9;
    uVar7 = (undefined1)((ushort)puVar9 >> 8);
    pcVar5 = (char *)CONCAT11(uVar7,cVar3 + '\x02');
    *pcVar5 = *(char *)CONCAT11(uVar7,cVar3 + 6U) + *pcVar5;
    pbVar6 = (byte *)CONCAT11(uVar7,cVar3 + '\x03');
    bVar2 = *(char *)CONCAT11(uVar7,cVar3 + '\a') + *pbVar6 + (0xfe < (byte)(cVar3 + 6U));
    *pbVar6 = bVar2;
    if ((bVar2 < 0xa8) || (0xe7 < bVar2)) {
      pcVar5 = (char *)CONCAT11(uVar7,cVar3 + '\x04');
      *pcVar5 = *(char *)CONCAT11(uVar7,cVar3 + 8U) + *pcVar5;
      pcVar5 = (char *)CONCAT11(uVar7,cVar3 + '\x05');
      cVar1 = *(char *)CONCAT11(uVar7,cVar3 + '\t') + *pcVar5 + (0xfe < (byte)(cVar3 + 8U));
      *pcVar5 = cVar1;
      bVar2 = cVar1 - 8;
      if (bVar2 < 0xf0) {
        return bVar2;
      }
      pbVar6 = (byte *)CONCAT11(uVar7,cVar3 + '\x03');
    }
    *pbVar6 = 0xe0;
    *(undefined1 *)CONCAT11((char)((ushort)pbVar6 >> 8),(char)pbVar6 + -3) = 0;
    return bVar2;
  }
  if (DAT_ram_e200 != '\x02') {
    if (DAT_ram_e200 != '\x03') {
      DAT_ram_e202 = 0xe0;
      DAT_ram_e358 = 0xe0;
      return 0x58;
    }
    DAT_ram_e20d = DAT_ram_e20d + -1;
    if (-1 < DAT_ram_e20d) {
      DAT_ram_e358 = DAT_ram_e203;
      uRame359 = DAT_ram_e205;
      cVar3 = DAT_ram_e20d * '\x02' + 0x50;
      pbVar6 = (byte *)CONCAT11(0x82,cVar3);
      if (0xaf < (byte)(DAT_ram_e20d * '\x02')) {
        pbVar6 = (byte *)CONCAT11(0x83,cVar3);
      }
      bRame35a = *pbVar6;
      bRame35b = pbVar6[1];
      if (bRame35a != 0x44) {
        return bRame35a;
      }
      DAT_ram_e358 = DAT_ram_e203 + 5U;
      return DAT_ram_e203 + 5U;
    }
    DAT_ram_e200 = 0;
    return 0;
  }
  if (1 < DAT_ram_e600) {
    return DAT_ram_e600;
  }
  puVar8 = &DAT_ram_e200;
  bVar2 = 0;
  cVar3 = DAT_ram_e60c;
  if (DAT_ram_e60c == '\0') {
LAB_ram_8325:
    FUN_ram_877f(cVar3);
    if (!(bool)(bVar2 & 1)) {
      *puVar9 = 0;
      puVar9[3] = 0xe0;
      if (DAT_ram_e60c == '\x03') {
        return 3;
      }
      if (DAT_ram_e600 == 2) {
        return 2;
      }
      DAT_ram_e600 = 2;
      DAT_ram_e60b = 0x90;
      bVar2 = FUN_ram_490c(0xae);
      return bVar2;
    }
  }
  else {
    if (DAT_ram_e60c == '\x01') {
      FUN_ram_8858();
      bVar2 = DAT_ram_e602 < 9;
      DAT_ram_e0d0 = DAT_ram_e602 - 9;
      DAT_ram_e0d1 = DAT_ram_e604;
      cVar3 = FUN_ram_88d3(0x810);
      if (!(bool)(bVar2 & 1)) {
        FUN_ram_490c(0x11);
        *puVar8 = 0;
        puVar8[3] = 0xe0;
        cVar3 = '\x01';
        if (6 < (byte)puVar9[1]) {
          cVar3 = '\x04';
        }
        DAT_ram_e60f = DAT_ram_e60f - cVar3;
        if (-1 < (char)DAT_ram_e60f) {
          return DAT_ram_e60f;
        }
        DAT_ram_e60f = 0;
        DAT_ram_e60c = 0;
        return 0;
      }
      goto LAB_ram_8325;
    }
    cVar3 = DAT_ram_e60c + -2;
    bVar2 = 0;
    if (cVar3 != '\0') goto LAB_ram_8325;
  }
  if (3 < DAT_ram_e3b0) {
    return DAT_ram_e3b0;
  }
  if (puVar9[1] != 6) {
    return puVar9[1];
  }
  bVar2 = DAT_ram_e620 >> 7;
  if (!(bool)bVar2) {
    FUN_ram_8788(DAT_ram_e620 << 1);
    pbVar6 = &DAT_ram_e625;
    if ((bVar2 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar4 = DAT_ram_e630 >> 7;
  if (!(bool)bVar4) {
    FUN_ram_8788(DAT_ram_e630 << 1 | bVar2 & 1);
    pbVar6 = &DAT_ram_e635;
    if ((bVar4 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar2 = DAT_ram_e640 >> 7;
  bVar4 = DAT_ram_e640 << 1 | bVar4 & 1;
  if (!(bool)bVar2) {
    bVar4 = FUN_ram_8788(bVar4);
    pbVar6 = &DAT_ram_e645;
    if (!(bool)(bVar2 & 1)) {
LAB_ram_83c1:
      if (*pbVar6 >> 1 < 3) {
        pbVar6 = (byte *)CONCAT11((char)((ushort)pbVar6 >> 8),(char)pbVar6 + -5);
        *pbVar6 = *pbVar6 | 0x80;
      }
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0x4d);
    }
  }
  return bVar4;
}



/* ram:8211 FUN_ram_8211 -- automatic, unverified */

char FUN_ram_8211(char param_1,undefined1 *param_2)

{
  char cVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 uVar5;
  char *pcVar4;
  undefined1 *puVar6;
  
  cVar1 = param_2[0xd] + -1;
  param_2[0xd] = cVar1;
  if (cVar1 < '\0') {
    *param_2 = 0;
    return param_1;
  }
  cVar1 = param_2[0xd];
  cVar2 = DAT_ram_e0c1 * '\x04';
  *(undefined1 *)CONCAT11(0xe3,cVar2 + 'X') = param_2[3];
  *(undefined1 *)CONCAT11(0xe3,cVar2 + 'Y') = param_2[5];
  puVar6 = (undefined1 *)CONCAT11(0xe3,cVar2 + 'Z');
  cVar2 = cVar1 * '\x02' + 0x50;
  puVar3 = (undefined1 *)CONCAT11(0x82,cVar2);
  if (0xaf < (byte)(cVar1 * '\x02')) {
    puVar3 = (undefined1 *)CONCAT11(0x83,cVar2);
  }
  *puVar6 = *puVar3;
  puVar6[1] = puVar3[1];
  cVar2 = (char)(puVar6 + 2);
  uVar5 = (undefined1)((ushort)(puVar6 + 2) >> 8);
  cVar1 = *(char *)CONCAT11(uVar5,cVar2 + -2);
  pcVar4 = (char *)CONCAT11(uVar5,cVar2 + -4);
  if (cVar1 != 'D') {
    return cVar1;
  }
  cVar1 = *pcVar4 + '\x05';
  *pcVar4 = cVar1;
  return cVar1;
}



/* ram:825e FUN_ram_825e -- automatic, unverified */

void FUN_ram_825e(void)

{
  byte bVar1;
  char cVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_e200;
  bVar1 = 0;
  cVar2 = '\b';
  while ((*pcVar3 != '\0' || (bVar1 = bVar1 + 1, bVar1 < DAT_ram_e0c3))) {
    pcVar3 = (char *)CONCAT11((char)((ushort)pcVar3 >> 8),(char)pcVar3 + '\x10');
    cVar2 = cVar2 + -1;
    if (cVar2 == '\0') {
      return;
    }
  }
  return;
}



/* ram:8278 FUN_ram_8278 -- automatic, unverified */

void FUN_ram_8278(short param_1)

{
  byte bVar1;
  char cVar2;
  byte bVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  
  puVar4 = (undefined1 *)CONCAT11(0xe3,DAT_ram_e0c1 * '\x04' + 'X');
  puVar5 = (undefined1 *)CONCAT11((char)((ushort)param_1 >> 8),(char)param_1 + '\x03');
  *puVar4 = *puVar5;
  puVar4[1] = puVar5[2];
  puVar4[2] = *(undefined1 *)(param_1 + 10);
  puVar4[3] = 0xf;
  bVar1 = *(byte *)(param_1 + 1);
  if ((bVar1 != 6) && (bVar1 != 0xc)) {
    return;
  }
  *(char *)(param_1 + 0xd) = *(char *)(param_1 + 0xd) + '\x01';
  bVar3 = *(byte *)(param_1 + 0xd);
  if (*(char *)(param_1 + 1) != '\x06') {
    bVar3 = bVar3 >> 1;
  }
  bVar1 = (bVar3 & 3) + (bVar1 & 2) * '\x02';
  cVar2 = bVar1 - 0x40;
  puVar5 = (undefined1 *)CONCAT11(0x82,cVar2);
  if (0x3f < bVar1) {
    puVar5 = (undefined1 *)CONCAT11(0x83,cVar2);
  }
  puVar4[2] = *puVar5;
  return;
}



/* ram:82c8 FUN_ram_82c8 -- automatic, unverified */

void FUN_ram_82c8(char *param_1,short param_2)

{
  undefined1 *puVar1;
  char cVar2;
  undefined1 uVar4;
  undefined1 *puVar3;
  
  *param_1 = *param_1 + '\x01';
  cVar2 = (char)param_1;
  uVar4 = (undefined1)((ushort)param_1 >> 8);
  *(undefined1 *)CONCAT11(uVar4,cVar2 + '\x01') = DAT_ram_e0c6;
  *(undefined1 *)CONCAT11(uVar4,cVar2 + '\x03') = *(undefined1 *)(param_2 + 3);
  *(undefined1 *)CONCAT11(uVar4,cVar2 + '\x05') = *(undefined1 *)(param_2 + 5);
  puVar1 = (undefined1 *)CONCAT11(uVar4,cVar2 + '\n');
  puVar3 = (undefined1 *)CONCAT11(0x83,param_1[1]);
  if (param_1[1] == '\0') {
    puVar3 = (undefined1 *)0x8400;
  }
  *puVar1 = *puVar3;
  puVar1 = puVar1 + 1;
  *puVar1 = 0xf;
  if (param_1[1] != '\v') {
    return;
  }
  *puVar1 = 8;
  return;
}



/* ram:830f FUN_ram_830f -- automatic, unverified */

void FUN_ram_830f(undefined1 *param_1)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  byte *pbVar4;
  undefined1 *puVar5;
  
  if (1 < DAT_ram_e600) {
    return;
  }
  bVar1 = 0;
  cVar3 = DAT_ram_e60c;
  puVar5 = param_1;
  if (DAT_ram_e60c == '\0') {
LAB_ram_8325:
    FUN_ram_877f(cVar3);
    if (!(bool)(bVar1 & 1)) {
      *puVar5 = 0;
      puVar5[3] = 0xe0;
      if (DAT_ram_e60c == '\x03') {
        return;
      }
      if (DAT_ram_e600 == 2) {
        return;
      }
      DAT_ram_e600 = 2;
      DAT_ram_e60b = 0x90;
      FUN_ram_490c(0xae);
      return;
    }
  }
  else {
    if (DAT_ram_e60c == '\x01') {
      FUN_ram_8858();
      bVar1 = DAT_ram_e602 < 9;
      DAT_ram_e0d0 = DAT_ram_e602 - 9;
      DAT_ram_e0d1 = DAT_ram_e604;
      cVar3 = FUN_ram_88d3(0x810);
      if (!(bool)(bVar1 & 1)) {
        FUN_ram_490c(0x11);
        *param_1 = 0;
        param_1[3] = 0xe0;
        cVar3 = '\x01';
        if (6 < (byte)puVar5[1]) {
          cVar3 = '\x04';
        }
        DAT_ram_e60f = DAT_ram_e60f - cVar3;
        if (-1 < DAT_ram_e60f) {
          return;
        }
        DAT_ram_e60f = 0;
        DAT_ram_e60c = 0;
        return;
      }
      goto LAB_ram_8325;
    }
    cVar3 = DAT_ram_e60c + -2;
    bVar1 = 0;
    if (cVar3 != '\0') goto LAB_ram_8325;
  }
  if (3 < DAT_ram_e3b0) {
    return;
  }
  if (puVar5[1] != '\x06') {
    return;
  }
  bVar1 = DAT_ram_e620 >> 7;
  if (!(bool)bVar1) {
    FUN_ram_8788(DAT_ram_e620 << 1);
    pbVar4 = &DAT_ram_e625;
    if ((bVar1 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar2 = DAT_ram_e630 >> 7;
  if (!(bool)bVar2) {
    FUN_ram_8788(DAT_ram_e630 << 1 | bVar1 & 1);
    pbVar4 = &DAT_ram_e635;
    if ((bVar2 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar1 = DAT_ram_e640 >> 7;
  if (!(bool)bVar1) {
    FUN_ram_8788(DAT_ram_e640 << 1 | bVar2 & 1);
    pbVar4 = &DAT_ram_e645;
    if (!(bool)(bVar1 & 1)) {
LAB_ram_83c1:
      if (*pbVar4 >> 1 < 3) {
        pbVar4 = (byte *)CONCAT11((char)((ushort)pbVar4 >> 8),(char)pbVar4 + -5);
        *pbVar4 = *pbVar4 | 0x80;
      }
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0x4d);
    }
  }
  return;
}



/* ram:8319 FUN_ram_8319 -- automatic, unverified */

void FUN_ram_8319(undefined1 *param_1,undefined1 *param_2)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  byte *pbVar4;
  
  bVar1 = 0;
  cVar3 = DAT_ram_e60c;
  if (DAT_ram_e60c == '\0') {
LAB_ram_8325:
    FUN_ram_877f(cVar3);
    if (!(bool)(bVar1 & 1)) {
      *param_1 = 0;
      param_1[3] = 0xe0;
      if (DAT_ram_e60c == '\x03') {
        return;
      }
      if (DAT_ram_e600 == '\x02') {
        return;
      }
      DAT_ram_e600 = 2;
      DAT_ram_e60b = 0x90;
      FUN_ram_490c(0xae);
      return;
    }
  }
  else {
    if (DAT_ram_e60c == '\x01') {
      FUN_ram_8858();
      bVar1 = DAT_ram_e602 < 9;
      DAT_ram_e0d0 = DAT_ram_e602 - 9;
      DAT_ram_e0d1 = DAT_ram_e604;
      cVar3 = FUN_ram_88d3(0x810);
      if (!(bool)(bVar1 & 1)) {
        FUN_ram_490c(0x11);
        *param_2 = 0;
        param_2[3] = 0xe0;
        cVar3 = '\x01';
        if (6 < (byte)param_1[1]) {
          cVar3 = '\x04';
        }
        DAT_ram_e60f = DAT_ram_e60f - cVar3;
        if (-1 < DAT_ram_e60f) {
          return;
        }
        DAT_ram_e60f = 0;
        DAT_ram_e60c = 0;
        return;
      }
      goto LAB_ram_8325;
    }
    cVar3 = DAT_ram_e60c + -2;
    bVar1 = 0;
    if (cVar3 != '\0') goto LAB_ram_8325;
  }
  if (3 < DAT_ram_e3b0) {
    return;
  }
  if (param_1[1] != '\x06') {
    return;
  }
  bVar1 = DAT_ram_e620 >> 7;
  if (!(bool)bVar1) {
    FUN_ram_8788(DAT_ram_e620 << 1);
    pbVar4 = &DAT_ram_e625;
    if ((bVar1 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar2 = DAT_ram_e630 >> 7;
  if (!(bool)bVar2) {
    FUN_ram_8788(DAT_ram_e630 << 1 | bVar1 & 1);
    pbVar4 = &DAT_ram_e635;
    if ((bVar2 & 1) == 0) goto LAB_ram_83c1;
  }
  bVar1 = DAT_ram_e640 >> 7;
  if (!(bool)bVar1) {
    FUN_ram_8788(DAT_ram_e640 << 1 | bVar2 & 1);
    pbVar4 = &DAT_ram_e645;
    if (!(bool)(bVar1 & 1)) {
LAB_ram_83c1:
      if (*pbVar4 >> 1 < 3) {
        pbVar4 = (byte *)CONCAT11((char)((ushort)pbVar4 >> 8),(char)pbVar4 + -5);
        *pbVar4 = *pbVar4 | 0x80;
      }
                    /* WARNING: Subroutine does not return */
      FUN_ram_4901(0x4d);
    }
  }
  return;
}



/* ram:8338 FUN_ram_8338 -- automatic, unverified */

void FUN_ram_8338(void)

{
  if (DAT_ram_e600 == '\x02') {
    return;
  }
  DAT_ram_e600 = 2;
  DAT_ram_e60b = 0x90;
  FUN_ram_490c(0xae);
  return;
}



/* ram:83e1 FUN_ram_83e1 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_83e1(void)

{
  char cVar1;
  undefined2 in_AF;
  byte bVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  
  bVar2 = (byte)((ushort)in_AF >> 8);
  if (0x40 < bVar2) {
    if (bVar2 < 0x80) {
      bVar2 = 0x80 - bVar2;
    }
    else if (bVar2 < 0xc0) {
      bVar2 = bVar2 + 0x80;
    }
    else {
      bVar2 = -bVar2;
    }
  }
  puVar3 = (undefined1 *)CONCAT11(0x84,bVar2 + 0x8f);
  if (0x70 < bVar2) {
    puVar3 = (undefined1 *)CONCAT11(0x85,bVar2 + 0x8f);
  }
  cVar1 = -bVar2 + -0x31;
  puVar4 = (undefined1 *)CONCAT11(0x84,cVar1);
  if (0x70 < (byte)(-bVar2 + 0x40)) {
    puVar4 = (undefined1 *)CONCAT11(0x85,cVar1);
  }
  _DAT_ram_e0cf = CONCAT11(*puVar3,*puVar4);
  return;
}



/* ram:84e0 FUN_ram_84e0 -- automatic, unverified */

char FUN_ram_84e0(short param_1)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  ushort uVar6;
  ushort *puVar7;
  
  cVar4 = '\x01';
  cVar1 = (DAT_ram_e602 + 0x10U) - *(byte *)(param_1 + 3);
  cVar3 = '\0';
  if ((cVar1 != '\0') && (cVar3 = cVar1, (byte)(DAT_ram_e602 + 0x10U) < *(byte *)(param_1 + 3))) {
    cVar4 = '\0';
    cVar3 = -cVar1;
  }
  cVar1 = (DAT_ram_e604 + 0x10U) - *(byte *)(param_1 + 5);
  cVar2 = '\0';
  cVar5 = '\0';
  if ((cVar1 != '\0') &&
     (cVar2 = cVar1, cVar5 = '\0', (byte)(DAT_ram_e604 + 0x10U) < *(byte *)(param_1 + 5))) {
    cVar5 = '\x01';
    cVar2 = -cVar1;
  }
  cVar1 = cVar4 + cVar5;
  if ((cVar1 == '\x01') && (cVar4 == '\x01')) {
    cVar1 = '\x03';
  }
  uVar6 = CONCAT11(cVar1,cVar5);
  FUN_ram_8569(cVar2,cVar3);
  puVar7 = (ushort *)&DAT_ram_84d0;
  cVar4 = '@';
  do {
    if (*puVar7 < uVar6) break;
    puVar7 = puVar7 + 1;
    cVar4 = cVar4 + -8;
  } while (cVar4 != '\0');
  if (cVar1 == '\x01') {
    return -0x80 - cVar4;
  }
  if (cVar1 == '\x02') {
    return cVar4 + -0x80;
  }
  if (cVar1 != '\x03') {
    return cVar4;
  }
  return -cVar4;
}



/* ram:8554 FUN_ram_8554 -- automatic, unverified */

byte FUN_ram_8554(undefined2 param_1,ushort param_2)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  byte bVar4;
  
  bVar4 = (byte)((ushort)param_1 >> 8);
  cVar3 = '\b';
  do {
    bVar1 = CARRY2(param_2,param_2);
    param_2 = param_2 * 2 + (ushort)bVar1;
    bVar2 = (byte)(param_2 >> 8);
    if ((bVar1) || (bVar4 <= bVar2)) {
      param_2 = CONCAT11(bVar2 - bVar4,(char)param_2);
      bVar2 = 0;
    }
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return bVar2;
}



/* ram:8569 FUN_ram_8569 -- automatic, unverified */

undefined1 FUN_ram_8569(undefined2 param_1)

{
  undefined1 uVar1;
  
  if ((char)((ushort)param_1 >> 8) != '\0') {
    FUN_ram_8554(0);
    uVar1 = FUN_ram_8554(0);
    return uVar1;
  }
  return 0xff;
}



/* ram:857f FUN_ram_857f -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_857f(undefined2 param_1)

{
  byte bVar1;
  char *pcVar2;
  
  DAT_ram_e0c6 = (char)((ushort)param_1 >> 8);
  _DAT_ram_e0c7 = 0x865c;
  pcVar2 = (char *)CONCAT11(0x87,DAT_ram_e0c6 + 'F');
  if (0xb8 < (byte)(DAT_ram_e0c6 - 1U)) {
    pcVar2 = (char *)CONCAT11(0x88,DAT_ram_e0c6 + 'F');
  }
  DAT_ram_e0c3 = *pcVar2;
  DAT_ram_e0c2 = FUN_ram_84e0();
  bVar1 = 0;
  DAT_ram_e0c4 = 0;
  FUN_ram_825e();
  if ((bVar1 & 1) != 0) {
    if ((DAT_ram_e0c6 == '\x05') || (DAT_ram_e0c6 == '\n')) {
      DAT_ram_e0c2 = -(DAT_ram_e0c3 * '\x06' - DAT_ram_e0c2);
    }
    DAT_ram_e0c1 = '\0';
    pcVar2 = &DAT_ram_e200;
    do {
      if (*pcVar2 == '\0') {
        FUN_ram_82c8(0,0x85f8);
                    /* WARNING: Subroutine does not return */
        inline_dispatch(DAT_ram_e0c6 + -1);
      }
      pcVar2 = pcVar2 + 0x10;
      DAT_ram_e0c1 = DAT_ram_e0c1 + '\x01';
    } while (DAT_ram_e0c1 != '\b');
    return;
  }
  return;
}



/* ram:860f state_860f -- automatic, unverified */

void state_860f(short param_1)

{
  char cVar1;
  undefined1 *puVar2;
  
  cVar1 = (DAT_ram_e0c2 >> 4) + 0x26;
  puVar2 = (undefined1 *)CONCAT11(0x86,cVar1);
  if (0xd9 < DAT_ram_e0c2 >> 4) {
    puVar2 = (undefined1 *)CONCAT11(0x87,cVar1);
  }
  *(undefined1 *)(param_1 + 10) = *puVar2;
  state_8646();
  return;
}



/* ram:8636 state_8636 -- automatic, unverified */

void state_8636(void)

{
  DAT_ram_e0c2 = DAT_ram_e0c2 + '\n';
  state_8646();
  return;
}



/* ram:863f state_863f -- automatic, unverified */

void state_863f(void)

{
  DAT_ram_e0c2 = DAT_ram_e0c2 + '\x10';
  FUN_ram_86b5();
  FUN_ram_871e();
  return;
}



/* ram:8646 state_8646 -- automatic, unverified */

void state_8646(void)

{
  FUN_ram_86b5();
  FUN_ram_871e();
  return;
}



/* ram:864c state_864c -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_864c(void)

{
  undefined1 uVar1;
  undefined1 *puVar2;
  
  uVar1 = *_DAT_ram_e0c7;
  puVar2 = _DAT_ram_e0c7 + 2;
  _DAT_ram_e0c7 = _DAT_ram_e0c7 + 4;
  FUN_ram_871e(uVar1,*puVar2);
  return;
}



/* ram:8674 state_8674 -- automatic, unverified */

void state_8674(short param_1)

{
  char *pcVar1;
  
  pcVar1 = &DAT_ram_868c;
  FUN_ram_405b(DAT_ram_e0c4);
  *(char *)(param_1 + 5) = *pcVar1 + *(char *)(param_1 + 5);
  FUN_ram_871e(0x300,0);
  return;
}



/* ram:86b5 FUN_ram_86b5 -- automatic, unverified */

byte FUN_ram_86b5(ushort param_1,short param_2)

{
  char cVar1;
  byte bVar2;
  byte *pbVar3;
  
  FUN_ram_83e1(DAT_ram_e0c2);
  bVar2 = (*(char *)(param_2 + 1) + -1) * '\x02';
  cVar1 = bVar2 + 0x2b;
  pbVar3 = (byte *)CONCAT11(0x87,cVar1);
  if (0xd4 < bVar2) {
    pbVar3 = (byte *)CONCAT11(0x88,cVar1);
  }
  if ((DAT_ram_e068 != '\0') || (3 < DAT_ram_e062)) {
    pbVar3 = pbVar3 + 1;
  }
  if ((*pbVar3 & 0xf) != 2) {
    bVar2 = (*pbVar3 & 0xf) - 3;
    if (bVar2 != 0) {
      return bVar2;
    }
    return bVar2;
  }
  if (-1 < (char)*pbVar3) {
    return *pbVar3 << 1 | CARRY2(param_1,param_1);
  }
  return DAT_ram_e0cf;
}



/* ram:871e FUN_ram_871e -- automatic, unverified */

void FUN_ram_871e(undefined2 param_1,undefined2 param_2,short param_3)

{
  *(char *)(param_3 + 6) = (char)param_1;
  *(char *)(param_3 + 7) = (char)((ushort)param_1 >> 8);
  *(char *)(param_3 + 8) = (char)param_2;
  *(char *)(param_3 + 9) = (char)((ushort)param_2 >> 8);
  return;
}



/* ram:8755 FUN_ram_8755 -- automatic, unverified */

void FUN_ram_8755(void)

{
  FUN_ram_87a2();
  FUN_ram_888f();
  FUN_ram_88d3();
  return;
}



/* ram:875e FUN_ram_875e -- automatic, unverified */

void FUN_ram_875e(void)

{
  FUN_ram_8814();
  FUN_ram_888f();
  FUN_ram_88d3();
  return;
}



/* ram:8761 FUN_ram_8761 -- automatic, unverified */

void FUN_ram_8761(void)

{
  FUN_ram_888f();
  FUN_ram_88d3();
  return;
}



/* ram:8767 FUN_ram_8767 -- automatic, unverified */

void FUN_ram_8767(char param_1,short param_2)

{
  if ((0xcf < *(byte *)(param_2 + 3)) && (*(byte *)(param_2 + 3) < 0xfa)) {
    return;
  }
  FUN_ram_88a4(param_1 + '\x02');
  FUN_ram_87a2();
  FUN_ram_88d3();
  return;
}



/* ram:877f FUN_ram_877f -- automatic, unverified */

void FUN_ram_877f(void)

{
  FUN_ram_888f();
  FUN_ram_8858();
  FUN_ram_88d3();
  return;
}



/* ram:8788 FUN_ram_8788 -- automatic, unverified */

void FUN_ram_8788(byte *param_1)

{
  byte in_F;
  
  if (!(bool)(*param_1 >> 7)) {
    FUN_ram_88a4(*param_1 << 1 | in_F & 1,(char)param_1 + '\x02');
    FUN_ram_8858();
    FUN_ram_88d3();
    return;
  }
  return;
}



/* ram:8797 FUN_ram_8797 -- automatic, unverified */

void FUN_ram_8797(char param_1)

{
  FUN_ram_88a4(param_1 + '\x02');
  FUN_ram_8804();
  FUN_ram_88d3();
  return;
}



/* ram:87a2 FUN_ram_87a2 -- automatic, unverified */

char FUN_ram_87a2(short param_1)

{
  char cVar1;
  byte bVar2;
  short sVar3;
  
  bVar2 = (*(byte *)(param_1 + 1) & 0xf) * '\x04';
  cVar1 = bVar2 - 0x3c;
  sVar3 = CONCAT11(0x87,cVar1);
  if (0x3b < bVar2) {
    sVar3 = CONCAT11(0x88,cVar1);
  }
  return *(char *)(param_1 + 5) + *(char *)(sVar3 + 1);
}



/* ram:8804 FUN_ram_8804 -- automatic, unverified */

char FUN_ram_8804(short param_1)

{
  return *(char *)(param_1 + 7) + '\x01';
}



/* ram:8814 FUN_ram_8814 -- automatic, unverified */

char FUN_ram_8814(void)

{
  char cVar1;
  byte bVar2;
  short sVar3;
  
  bVar2 = (DAT_ram_e065 & 0xf) * '\x04';
  cVar1 = bVar2 + 0x38;
  sVar3 = CONCAT11(0x88,cVar1);
  if (199 < bVar2) {
    sVar3 = CONCAT11(0x89,cVar1);
  }
  return DAT_ram_e3d5 + *(char *)(sVar3 + 3);
}



/* ram:8858 FUN_ram_8858 -- automatic, unverified */

void FUN_ram_8858(void)

{
  return;
}



/* ram:888f FUN_ram_888f -- automatic, unverified */

void FUN_ram_888f(void)

{
  DAT_ram_e0d0 = DAT_ram_e602 + '\x03';
  DAT_ram_e0d1 = DAT_ram_e604 + '\x03';
  return;
}



/* ram:88a4 FUN_ram_88a4 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_88a4(undefined1 *param_1)

{
  _DAT_ram_e0d0 =
       CONCAT11(*(undefined1 *)CONCAT11((char)((ushort)param_1 >> 8),(char)param_1 + '\x02'),
                *param_1);
  return;
}



/* ram:88d3 FUN_ram_88d3 -- automatic, unverified */

byte FUN_ram_88d3(undefined2 param_1,char param_2,undefined2 param_3)

{
  byte bVar1;
  char cVar2;
  
  cVar2 = (char)((ushort)param_3 >> 8);
  bVar1 = (char)((ushort)param_1 >> 8) + cVar2;
  if (bVar1 < (byte)((DAT_ram_e0d0 + cVar2) - param_2)) {
    return bVar1;
  }
  return (char)param_1 + (char)param_3;
}



/* ram:88e7 FUN_ram_88e7 -- automatic, unverified */

undefined1 FUN_ram_88e7(void)

{
  if (DAT_ram_e3b0 == '\0') {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  inline_dispatch(DAT_ram_e065);
}



/* ram:8902 FUN_ram_8902 -- automatic, unverified */

void FUN_ram_8902(void)

{
  char cVar1;
  undefined1 *puVar2;
  
  DAT_ram_e3b0 = 5;
  DAT_ram_e3c0 = 5;
  cVar1 = '\n';
  puVar2 = &DAT_ram_e200;
  do {
    *puVar2 = 0;
    puVar2[3] = 0xe0;
    puVar2 = puVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_ram_8a2a();
  DAT_ram_e3b1 = 0x40;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x17);
}



/* ram:8904 FUN_ram_8904 -- automatic, unverified */

void FUN_ram_8904(undefined1 param_1)

{
  char cVar1;
  undefined1 *puVar2;
  
  cVar1 = '\n';
  puVar2 = &DAT_ram_e200;
  DAT_ram_e3b0 = param_1;
  DAT_ram_e3c0 = param_1;
  do {
    *puVar2 = 0;
    puVar2[3] = 0xe0;
    puVar2 = puVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_ram_8a2a();
  DAT_ram_e3b1 = 0x40;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x17);
}



/* ram:8929 FUN_ram_8929 -- automatic, unverified */

undefined1 FUN_ram_8929(void)

{
  FUN_ram_98db();
  return DAT_ram_e3bf;
}



/* ram:8931 FUN_ram_8931 -- automatic, unverified */

void FUN_ram_8931(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  char *pcVar9;
  char *pcVar10;
  
  DAT_ram_e3ba = '\n';
  pcVar5 = &DAT_ram_e320;
  pcVar10 = &DAT_ram_8999;
  uVar1 = 0x440a;
  uVar2 = 0x405;
  uVar7 = 0x4806;
  uVar8 = 0;
  pcVar9 = &DAT_ram_e320;
  do {
    uVar4 = uVar2;
    uVar3 = uVar1;
    *pcVar5 = DAT_ram_e3d3 + *pcVar10 + (char)uVar4;
    pcVar5[1] = DAT_ram_e3d5 + pcVar10[1] + (char)((ushort)uVar4 >> 8);
    pcVar5[2] = (char)((ushort)uVar3 >> 8);
    pcVar5[3] = (char)uVar3;
    pcVar6 = pcVar5 + 4;
    pcVar5 = pcVar9 + 4;
    pcVar10 = pcVar10 + 2;
    DAT_ram_e3ba = DAT_ram_e3ba + -1;
    uVar1 = uVar7;
    uVar2 = uVar8;
    uVar7 = uVar3;
    uVar8 = uVar4;
    pcVar9 = pcVar6;
  } while (DAT_ram_e3ba != '\0');
  return;
}



/* ram:8979 FUN_ram_8979 -- automatic, unverified */

void FUN_ram_8979(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  char *pcVar9;
  char *pcVar10;
  
  DAT_ram_e3ba = '\n';
  pcVar5 = &DAT_ram_e320;
  pcVar10 = &DAT_ram_8999;
  uVar1 = 0x4c0a;
  uVar2 = 0x208;
  uVar7 = 0x5006;
  uVar8 = 0;
  pcVar9 = &DAT_ram_e320;
  do {
    uVar4 = uVar2;
    uVar3 = uVar1;
    *pcVar5 = DAT_ram_e3d3 + *pcVar10 + (char)uVar4;
    pcVar5[1] = DAT_ram_e3d5 + pcVar10[1] + (char)((ushort)uVar4 >> 8);
    pcVar5[2] = (char)((ushort)uVar3 >> 8);
    pcVar5[3] = (char)uVar3;
    pcVar6 = pcVar5 + 4;
    pcVar5 = pcVar9 + 4;
    pcVar10 = pcVar10 + 2;
    DAT_ram_e3ba = DAT_ram_e3ba + -1;
    uVar1 = uVar7;
    uVar2 = uVar8;
    uVar7 = uVar3;
    uVar8 = uVar4;
    pcVar9 = pcVar6;
  } while (DAT_ram_e3ba != '\0');
  return;
}



/* ram:8989 FUN_ram_8989 -- automatic, unverified */

void FUN_ram_8989(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  char *pcVar3;
  char *pcVar4;
  undefined2 uVar5;
  char *pcVar6;
  char *pcVar7;
  
  DAT_ram_e3ba = '\n';
  pcVar3 = &DAT_ram_e320;
  pcVar7 = &DAT_ram_8999;
  uVar1 = 0x5406;
  uVar5 = 0xff00;
  pcVar6 = &DAT_ram_e320;
  do {
    uVar2 = uVar1;
    *pcVar3 = DAT_ram_e3d3 + *pcVar7;
    pcVar3[1] = DAT_ram_e3d5 + pcVar7[1];
    pcVar3[2] = (char)((ushort)uVar2 >> 8);
    pcVar3[3] = (char)uVar2;
    pcVar4 = pcVar3 + 4;
    pcVar3 = pcVar6 + 4;
    pcVar7 = pcVar7 + 2;
    DAT_ram_e3ba = DAT_ram_e3ba + -1;
    uVar1 = uVar5;
    uVar5 = uVar2;
    pcVar6 = pcVar4;
  } while (DAT_ram_e3ba != '\0');
  return;
}



/* ram:89ad FUN_ram_89ad -- automatic, unverified */

void FUN_ram_89ad(undefined2 param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  
  cVar1 = (char)((ushort)param_1 >> 8);
  pcVar2 = &DAT_ram_e320;
  do {
    *pcVar2 = DAT_ram_e3d3 + *param_2;
    pcVar2[1] = DAT_ram_e3d5 + param_2[1];
    pcVar2[2] = param_2[2];
    pcVar2[3] = param_2[3];
    param_2 = param_2 + 4;
    pcVar2 = pcVar2 + 4;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:89cd FUN_ram_89cd -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_89cd(void)

{
  short sVar1;
  
  DAT_ram_e3b6 = -(DAT_ram_e3d3 - (char)_DAT_ram_e3b7);
  sVar1 = CONCAT11(DAT_ram_e3b6,(char)_DAT_ram_e3b7);
  FUN_ram_8a16(DAT_ram_e3b3);
  _DAT_ram_e3d6 = sVar1 + _DAT_ram_e3d6;
  _DAT_ram_e3d2 = _DAT_ram_e3d6 + _DAT_ram_e3d2;
  DAT_ram_e3b5 = -(DAT_ram_e3d5 - (char)((ushort)_DAT_ram_e3b7 >> 8));
  sVar1 = CONCAT11(DAT_ram_e3b5,(char)_DAT_ram_e3b7);
  FUN_ram_8a16(DAT_ram_e3b4);
  _DAT_ram_e3d8 = sVar1 + _DAT_ram_e3d8;
  _DAT_ram_e3d4 = _DAT_ram_e3d8 + _DAT_ram_e3d4;
  return;
}



/* ram:8a16 FUN_ram_8a16 -- automatic, unverified */

byte FUN_ram_8a16(byte param_1,ushort param_2,byte param_3)

{
  bool bVar1;
  char cVar2;
  byte bVar4;
  ushort uVar3;
  
  bVar4 = (byte)(param_2 >> 8);
  if ((param_2 & 0x8000) != 0) {
    bVar4 = -bVar4;
    FUN_ram_8a73();
    return ~bVar4;
  }
  uVar3 = (ushort)bVar4 << 8;
  cVar2 = '\b';
  do {
    bVar1 = CARRY2(uVar3,uVar3);
    uVar3 = uVar3 * 2;
    if (bVar1) {
      uVar3 = uVar3 + param_3;
    }
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return param_1;
}



/* ram:8a2a FUN_ram_8a2a -- automatic, unverified */

void FUN_ram_8a2a(void)

{
  undefined1 uVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  char *pcVar5;
  
  puVar4 = &DAT_ram_e358;
  pcVar5 = &DAT_ram_e200;
  cVar2 = '\n';
  do {
    if (*pcVar5 == '\0') {
      *puVar4 = 0xe0;
      puVar4 = (undefined1 *)CONCAT11((char)((ushort)puVar4 >> 8),(char)puVar4 + '\x04');
      uVar1 = 0x10;
    }
    else {
      puVar3 = (undefined1 *)CONCAT11((char)((ushort)pcVar5 >> 8),(char)pcVar5 + '\x03');
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar3 = (undefined1 *)CONCAT11((char)((ushort)puVar3 >> 8),(char)puVar3 + '\x01');
      puVar4[1] = *puVar3;
      puVar3 = puVar3 + 1;
      puVar3 = (undefined1 *)CONCAT11((char)((ushort)puVar3 >> 8),(char)puVar3 + '\x04');
      puVar4[2] = *puVar3;
      puVar4[3] = puVar3[1];
      puVar4 = puVar4 + 4;
      pcVar5 = puVar3 + 2;
      uVar1 = 4;
    }
    FUN_ram_405b(uVar1);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:8a5c FUN_ram_8a5c -- automatic, unverified */

void FUN_ram_8a5c(void)

{
  char cVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_ram_e200;
  cVar1 = '\n';
  do {
    if (*pcVar2 == '\x01') {
      FUN_ram_79a8(0);
    }
    pcVar2 = pcVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:8a73 FUN_ram_8a73 -- automatic, unverified */

void FUN_ram_8a73(ushort param_1,byte param_2)

{
  bool bVar1;
  char cVar2;
  
  param_1 = param_1 & 0xff00;
  cVar2 = '\b';
  do {
    bVar1 = CARRY2(param_1,param_1);
    param_1 = param_1 * 2;
    if (bVar1) {
      param_1 = param_1 + param_2;
    }
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:8a8b FUN_ram_8a8b -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_ram_8a8b(void)

{
  _DAT_ram_e3b1 = _DAT_ram_e3b1 + -1;
  return (byte)_DAT_ram_e3b1 | (byte)((ushort)_DAT_ram_e3b1 >> 8);
}



/* ram:8a95 state_8a95 -- automatic, unverified */

void state_8a95(void)

{
  char cVar1;
  
  cVar1 = DAT_ram_e3b0 + -1;
  if (cVar1 < '\0') {
    return;
  }
  if (cVar1 == '\x02') {
    FUN_ram_8dc6();
  }
                    /* WARNING: Subroutine does not return */
  inline_dispatch(cVar1);
}



/* ram:8ab4 state_8ab4 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_8ab4(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined2 uVar4;
  
  puVar3 = &DAT_ram_e3d0;
  puVar2 = &DAT_ram_e3d1;
  sVar1 = 0x3f;
  DAT_ram_e3d0 = 0;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  DAT_ram_e3d3 = 0xd8;
  DAT_ram_e3d5 = 0x70;
  puVar2 = (undefined1 *)0x8c14;
  FUN_ram_405b(DAT_ram_e065);
  DAT_ram_e3e0 = *puVar2;
  uVar4 = 0x8c1c;
  FUN_ram_4079();
  DAT_ram_e3f0 = 0x80;
  _DAT_ram_e3b1 = uVar4;
  FUN_ram_4412(0x1cc0,0xaf73);
  FUN_ram_4412(0x1e00,0xad17);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:8ae6 state_8ae6 -- automatic, unverified */

void state_8ae6(void)

{
  FUN_ram_4412(0x1cc0,0xaf73);
  FUN_ram_4412(0x1e00,0xad17);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:8afa state_8afa -- automatic, unverified */

void state_8afa(void)

{
  if ((DAT_ram_e090 & 0x3f) != 1) {
    return;
  }
  DAT_ram_e3d3 = DAT_ram_e3d3 + 8;
  if ((char)DAT_ram_e3d3 < '\0') {
    return;
  }
  if (DAT_ram_e3d3 < 0x20) {
    return;
  }
  FUN_ram_8d87();
  FUN_ram_8bd7();
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:8b1b state_8b1b -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_8b1b(void)

{
  bool bVar1;
  char cVar2;
  byte in_F;
  byte bVar3;
  undefined1 uVar4;
  short sVar5;
  
  FUN_ram_8a8b();
  if (!(bool)(in_F >> 6 & 1)) {
    bVar1 = (DAT_ram_e090 & 0x10) == 0;
    bVar3 = bVar1 << 6;
    if (!bVar1) {
      bVar3 = 0;
    }
    DAT_ram_e3e1 = !bVar1;
    FUN_ram_8cf3();
    FUN_ram_8cc2();
    FUN_ram_8c2e();
    FUN_ram_8929();
    if ((bVar3 >> 6 & 1) != 0) {
      DAT_ram_e583 = DAT_ram_e3d3 + '\f';
      DAT_ram_e585 = DAT_ram_e3d5 + '\n';
      if ((DAT_ram_e003 & 0x1f) == 0) {
        FUN_ram_405b(DAT_ram_e3e0);
        FUN_ram_857f(0xe580);
      }
      cVar2 = DAT_ram_e3db;
      if (DAT_ram_e3db != '\0') {
        DAT_ram_e3db = DAT_ram_e3db + -1;
        if (DAT_ram_e3db != '\0') {
          return;
        }
        FUN_ram_8bd7(cVar2);
        return;
      }
      if (DAT_ram_e3da != (byte)(((byte)(_DAT_ram_e3d4 >> 8) & 0xfc) + 0xc)) {
        _DAT_ram_e3d4 = _DAT_ram_e3d4 + _DAT_ram_e3d8;
        sVar5 = CONCAT11(0x20,(char)_DAT_ram_e3d4);
        uVar4 = 0x50;
        if (0x1fff < _DAT_ram_e3d4) {
          sVar5 = CONCAT11(0xbf,(char)_DAT_ram_e3d4);
          uVar4 = 0x88;
          if (_DAT_ram_e3d4 < 0xbf00) {
            return;
          }
        }
        _DAT_ram_e3d4 = sVar5;
        if (DAT_ram_e065 != '\0') {
          _DAT_ram_e3d8 = CONCAT11(-(char)((ushort)_DAT_ram_e3d8 >> 8),DAT_ram_e3d8);
          DAT_ram_e3da = uVar4;
          return;
        }
      }
      DAT_ram_e3db = 0x50;
      return;
    }
  }
  FUN_ram_8904(4);
  return;
}



/* ram:8bb0 state_8bb0 -- automatic, unverified */

void state_8bb0(void)

{
  if ((DAT_ram_e065 == '\x05') && ((DAT_ram_e3f0 & 0x80) == 0)) {
    FUN_ram_8d00();
  }
  FUN_ram_9401();
  DAT_ram_e3b1 = DAT_ram_e3b1 + -1;
  if (DAT_ram_e3b1 != '\0') {
    return;
  }
  DAT_ram_e3b1 = 0x30;
  FUN_ram_8db7();
  DAT_ram_e3d5 = DAT_ram_e3d5 + '\x12';
  FUN_ram_44cd();
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x6b);
}



/* ram:8bd7 FUN_ram_8bd7 -- automatic, unverified */

void FUN_ram_8bd7(void)

{
  byte bVar1;
  byte bVar2;
  char *pcVar3;
  
  bVar2 = DAT_ram_e604 & 0xfc;
  bVar1 = (DAT_ram_e3d5 & 0xfc) + 0xc;
  DAT_ram_e3d9 = '\0';
  if ((bVar1 != bVar2) && (DAT_ram_e3d9 = '\x01', bVar2 <= bVar1)) {
    DAT_ram_e3d9 = -1;
  }
  if ((DAT_ram_e065 != '\0') && (DAT_ram_e3d9 == '\0')) {
    DAT_ram_e3d9 = '\x01';
  }
  DAT_ram_e3d8 = 0;
  pcVar3 = &DAT_ram_8c25;
  FUN_ram_405b(DAT_ram_e3e0 * '\x03' + DAT_ram_e3d9 + '\x01');
  DAT_ram_e3da = *pcVar3 + bVar2;
  return;
}



/* ram:8c2e FUN_ram_8c2e -- automatic, unverified */

byte FUN_ram_8c2e(void)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char *pcVar4;
  undefined1 uVar5;
  short unaff_BC_;
  char *unaff_HL_;
  char *pcVar6;
  
  if (DAT_ram_e065 != 0) {
    return DAT_ram_e065;
  }
  if ((bool)(DAT_ram_e090 & 1)) {
    return DAT_ram_e090 >> 1;
  }
  pcVar6 = &DAT_ram_e590;
  cVar3 = '\x03';
  do {
    uVar5 = (undefined1)unaff_BC_;
    pcVar4 = &DAT_ram_8cbf;
    FUN_ram_405b(cVar3);
    cVar1 = *pcVar4;
    pcVar6[0x18] = pcVar6[0x18] + '\x01';
    if ((pcVar6[0x18] == cVar1) && (*pcVar6 == '\0')) {
      *pcVar6 = '\x01';
      pcVar6[1] = '\f';
      pcVar6[3] = '\x10';
      DAT_ram_e666 = DAT_ram_e666 + 1;
      pcVar6[5] = (DAT_ram_e666 & 1) + 0x7f;
    }
    FUN_ram_75ea(pcVar6[1]);
    FUN_ram_749a();
    FUN_ram_7452();
    pcVar4 = &DAT_ram_e338;
    FUN_ram_4060(cVar3 * '\b',unaff_HL_);
    unaff_HL_ = &DAT_ram_7896;
    if (pcVar6[10] != '\x1c') {
      unaff_HL_ = &DAT_ram_789e;
    }
    unaff_BC_ = CONCAT11(2,uVar5);
    do {
      cVar1 = pcVar6[3];
      if (*pcVar6 == '\0') {
        cVar1 = -0x36;
      }
      *pcVar4 = cVar1 + *unaff_HL_;
      pcVar4[1] = pcVar6[5] + unaff_HL_[1];
      cVar1 = unaff_HL_[2];
      pcVar4[2] = cVar1 - 0x40U;
      pcVar4[3] = unaff_HL_[3];
      pcVar4 = pcVar4 + 4;
      unaff_HL_ = unaff_HL_ + 4;
      cVar2 = (char)((ushort)(unaff_BC_ + -1) >> 8) + -1;
      unaff_BC_ = CONCAT11(cVar2,(char)(unaff_BC_ + -1));
    } while (cVar2 != '\0');
    pcVar6 = pcVar6 + 0x20;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return cVar1 - 0x40U;
}



/* ram:8cc2 FUN_ram_8cc2 -- automatic, unverified */

void FUN_ram_8cc2(void)

{
  if (DAT_ram_e065 != '\x04') {
    return;
  }
  if ((DAT_ram_e090 & 0x7f) == 0) {
    DAT_ram_e3e2 = DAT_ram_e3e2 + 1;
  }
  if ((DAT_ram_e3e2 & 1) == 0) {
    return;
  }
  if ((DAT_ram_e090 & 0x7f) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0x47);
  }
  FUN_ram_8df1(0x303,CONCAT11(DAT_ram_e3d5 + '\b',DAT_ram_e3d3 + '\b'),0xbf4a);
  return;
}



/* ram:8cf3 FUN_ram_8cf3 -- automatic, unverified */

void FUN_ram_8cf3(void)

{
  char cVar1;
  undefined1 *puVar2;
  char *pcVar3;
  undefined1 unaff_C_;
  
  if (DAT_ram_e065 != '\x05') {
    return;
  }
  if ((DAT_ram_e3f0 & 0x80) != 0) {
    if ((4 < (byte)((DAT_ram_e3d5 + '\x1a') - DAT_ram_e604)) && ((DAT_ram_e003 & 0x7f) != 0)) {
      return;
    }
    DAT_ram_e3f0 = 0;
    DAT_ram_e3f2 = 1;
    DAT_ram_e3f3 = DAT_ram_e3d5 + '\x18';
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0x5a);
  }
  bRame3f1 = bRame3f1 + 1;
  if ((bRame3f1 & 3) == 0) {
    DAT_ram_e3f2 = DAT_ram_e3f2 + 1;
  }
  puVar2 = &DAT_ram_e3f3;
  if (DAT_ram_e3f2 < 0x11) {
    FUN_ram_4060((DAT_ram_e3f2 - 0x10) * -2);
    FUN_ram_8df1(CONCAT11(*puVar2,0x50));
  }
  else if (0x14 < DAT_ram_e3f2) {
    DAT_ram_e3f0 = DAT_ram_e3f0 | 0x80;
    FUN_ram_43b0(0x3940,0x8e14);
    thunk_FUN_ram_64f9(unaff_C_,0xe9e0);
    thunk_FUN_ram_64f9();
    return;
  }
  if (1 < DAT_ram_e600) {
    return;
  }
  pcVar3 = &DAT_ram_e610;
  cVar1 = '\x06';
  while( true ) {
    if (((byte)(*pcVar3 + 0x95U) < 10) || ((byte)(*pcVar3 + 0x35U) < 10)) break;
    pcVar3 = pcVar3 + 1;
    cVar1 = cVar1 + -1;
    if (cVar1 == '\0') {
      return;
    }
  }
  FUN_ram_8338();
  return;
}



/* ram:8d00 FUN_ram_8d00 -- automatic, unverified */

void FUN_ram_8d00(short param_1)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  byte *pbVar4;
  undefined1 *puVar5;
  char *pcVar6;
  undefined1 unaff_C_;
  
  pbVar3 = (byte *)(param_1 + 1);
  *pbVar3 = *pbVar3 + 1;
  pbVar4 = (byte *)(param_1 + 2);
  if ((*pbVar3 & 3) == 0) {
    *pbVar4 = *pbVar4 + 1;
  }
  bVar1 = *pbVar4;
  puVar5 = (undefined1 *)(param_1 + 3);
  if (bVar1 < 0x11) {
    FUN_ram_4060((bVar1 - 0x10) * -2);
    FUN_ram_8df1(CONCAT11(*puVar5,0x50));
  }
  else if (0x14 < bVar1) {
    DAT_ram_e3f0 = DAT_ram_e3f0 | 0x80;
    FUN_ram_43b0(0x3940,0x8e14);
    thunk_FUN_ram_64f9(unaff_C_,0xe9e0);
    thunk_FUN_ram_64f9();
    return;
  }
  if (1 < DAT_ram_e600) {
    return;
  }
  pcVar6 = &DAT_ram_e610;
  cVar2 = '\x06';
  while( true ) {
    if (((byte)(*pcVar6 + 0x95U) < 10) || ((byte)(*pcVar6 + 0x35U) < 10)) break;
    pcVar6 = pcVar6 + 1;
    cVar2 = cVar2 + -1;
    if (cVar2 == '\0') {
      return;
    }
  }
  FUN_ram_8338();
  return;
}



/* ram:8d87 FUN_ram_8d87 -- automatic, unverified */

void FUN_ram_8d87(void)

{
  char *pcVar1;
  short sVar2;
  
  pcVar1 = &DAT_ram_8daf;
  FUN_ram_405b(DAT_ram_e065);
  if ((char)(*pcVar1 + '\x01') == '\0') {
    return;
  }
  DAT_ram_e08a = 0;
  sVar2 = -0x16d4;
  FUN_ram_6529(*pcVar1 + '\x01',2,0xe92c,pcVar1);
  FUN_ram_6529(2,sVar2 + 0x78);
  return;
}



/* ram:8db7 FUN_ram_8db7 -- automatic, unverified */

void FUN_ram_8db7(void)

{
  undefined1 *puVar1;
  char cVar2;
  ushort uVar3;
  undefined1 *puVar4;
  undefined1 unaff_C_;
  
  FUN_ram_43b0(0x3880);
  puVar4 = (undefined1 *)0xe920;
  uVar3 = CONCAT11(0xc0,unaff_C_);
  do {
    puVar1 = (undefined1 *)(uVar3 & 0xff);
    cVar2 = (char)(uVar3 >> 8) + -1;
    uVar3 = CONCAT11(cVar2,(char)uVar3);
    *puVar1 = *puVar4;
    puVar4 = puVar4 + 1;
  } while (cVar2 != '\0');
  return;
}



/* ram:8dc3 thunk_FUN_ram_64f9 -- automatic, unverified */

void thunk_FUN_ram_64f9(ushort param_1,undefined1 *param_2)

{
  char cVar1;
  
  cVar1 = (char)(param_1 >> 8);
  do {
    cVar1 = cVar1 + -1;
    *(undefined1 *)(param_1 & 0xff) = *param_2;
    param_2 = param_2 + 1;
  } while (cVar1 != '\0');
  return;
}



/* ram:8dc6 FUN_ram_8dc6 -- automatic, unverified */

void FUN_ram_8dc6(undefined2 param_1)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  char cVar4;
  ushort uVar5;
  
  FUN_ram_8db7();
  uVar5 = CONCAT11(DAT_ram_e3d5,DAT_ram_e3d3);
  FUN_ram_4079(DAT_ram_e3e0,0xbecc);
  FUN_ram_4071(DAT_ram_e3e1);
  uVar2 = 5;
  if (DAT_ram_e065 == '\a') {
    uVar2 = 6;
  }
  cVar4 = '\x06';
  if (DAT_ram_e065 == '\0') {
    cVar4 = '\x05';
  }
  uVar1 = FUN_ram_4372(DAT_ram_e065);
  do {
    uVar3 = uVar2;
    func_0x0053(uVar1);
    thunk_FUN_ram_64f9(CONCAT11(uVar3,DAT_ram_0007),param_1);
    uVar1 = FUN_ram_405b(0x20);
    if (0x3abf < uVar5) {
      return;
    }
    cVar4 = cVar4 + -1;
  } while (cVar4 != '\0');
  return;
}



/* ram:8dd1 FUN_ram_8dd1 -- automatic, unverified */

void FUN_ram_8dd1(ushort param_1,undefined2 param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  char cVar4;
  
  FUN_ram_4079(DAT_ram_e3e0,0xbecc);
  FUN_ram_4071(DAT_ram_e3e1);
  uVar2 = 5;
  if (DAT_ram_e065 == '\a') {
    uVar2 = 6;
  }
  cVar4 = '\x06';
  if (DAT_ram_e065 == '\0') {
    cVar4 = '\x05';
  }
  uVar1 = FUN_ram_4372(DAT_ram_e065);
  do {
    uVar3 = uVar2;
    func_0x0053(uVar1);
    thunk_FUN_ram_64f9(CONCAT11(uVar3,DAT_ram_0007),param_2);
    uVar1 = FUN_ram_405b(0x20);
    if (0x3abf < param_1) {
      return;
    }
    cVar4 = cVar4 + -1;
  } while (cVar4 != '\0');
  return;
}



/* ram:8df1 FUN_ram_8df1 -- automatic, unverified */

void FUN_ram_8df1(undefined2 param_1,ushort param_2,undefined2 param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  char cVar3;
  
  uVar1 = FUN_ram_4372();
  do {
    uVar2 = (char)param_1;
    func_0x0053(uVar1);
    thunk_FUN_ram_64f9(CONCAT11(uVar2,DAT_ram_0007),param_3);
    uVar1 = FUN_ram_405b(0x20);
    if (0x3abf < param_2) {
      return;
    }
    cVar3 = (char)((ushort)param_1 >> 8) + -1;
    param_1 = CONCAT11(cVar3,(char)param_1);
  } while (cVar3 != '\0');
  return;
}



/* ram:8e34 state_8e34 -- automatic, unverified */

void state_8e34(void)

{
  undefined2 uStack_2;
  
  if ((char)(DAT_ram_e3b0 - 1U) < '\0') {
    return;
  }
  if ((byte)(DAT_ram_e3b0 - 1U) < 4) {
    register0x44 = (BADSPACEBASE *)&uStack_2;
    uStack_2 = 0x900c;
  }
                    /* WARNING: Subroutine does not return */
  *(undefined2 *)((short)register0x44 + -2) = 0x8e44;
  inline_dispatch();
}



/* ram:8e54 state_8e54 -- automatic, unverified */

void state_8e54(void)

{
                    /* WARNING: Subroutine does not return */
  FUN_ram_940f();
}



/* ram:8e57 state_8e57 -- automatic, unverified */

void state_8e57(void)

{
  FUN_ram_440c(0xad69);
  FUN_ram_4412(0x1f80,0xabdc);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:8e69 state_8e69 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_8e69(void)

{
  _DAT_ram_e3b3 = 0x140;
  _DAT_ram_e3d8 = 0x180;
  _DAT_ram_e3b1 = 0xe10;
  _DAT_ram_e3d6 = 0x100;
  _DAT_ram_e3b7 = 0x7820;
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:8e75 state_8e75 -- automatic, unverified */

void state_8e75(void)

{
  byte in_F;
  
  FUN_ram_8a8b();
  if (!(bool)(in_F >> 6 & 1)) {
    FUN_ram_89cd();
    FUN_ram_8f43();
    FUN_ram_8929();
    if ((in_F >> 6 & 1) != 0) {
      FUN_ram_8ea2();
      if ((DAT_ram_e003 & 0xf) != 0) {
        return;
      }
      FUN_ram_8eeb(0,&DAT_ram_e3d0);
      return;
    }
  }
  FUN_ram_8902();
  DAT_ram_e3d3 = DAT_ram_e3d3 + -0x10;
  return;
}



/* ram:8ea2 FUN_ram_8ea2 -- automatic, unverified */

void FUN_ram_8ea2(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  char cVar1;
  char *pcVar2;
  
  if (DAT_ram_e600 == '\x02') {
    return;
  }
  pcVar2 = &DAT_ram_e200;
  cVar1 = '\b';
  do {
    if (*pcVar2 == '\x01') {
      FUN_ram_8319(0,param_1,param_2,param_3);
      FUN_ram_8ec5();
    }
    pcVar2 = pcVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:8ec5 FUN_ram_8ec5 -- automatic, unverified */

void FUN_ram_8ec5(undefined1 *param_1)

{
  byte in_F;
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  
  pbVar3 = &DAT_ram_e620;
  cVar2 = '\x03';
  do {
    bVar1 = *pbVar3 >> 7;
    if (!(bool)bVar1) {
      FUN_ram_8788(*pbVar3 << 1 | in_F & 1);
      if ((bVar1 & 1) == 0) {
        *pbVar3 = *pbVar3 | 0x80;
        *param_1 = 0;
        param_1[3] = 0xe0;
        return;
      }
    }
    in_F = (byte *)0xffef < pbVar3;
    pbVar3 = pbVar3 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:8eeb FUN_ram_8eeb -- automatic, unverified */

void FUN_ram_8eeb(void)

{
  char cVar1;
  undefined2 uVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_e200;
  cVar1 = '\x05';
  do {
    if (*pcVar3 == '\0') {
      *pcVar3 = '\x01';
      pcVar3[1] = '\x06';
      pcVar3[3] = DAT_ram_e3d3 + '\x10';
      cVar1 = DAT_ram_e3d5 + '\x04';
      pcVar3[5] = cVar1;
      pcVar3[6] = '\0';
      pcVar3[7] = '\0';
      pcVar3 = pcVar3 + 8;
      uVar2 = 0x180;
      if ((DAT_ram_e3cf & 2) == 0) {
        FUN_ram_8f3b();
      }
      *pcVar3 = (char)uVar2;
      pcVar3[1] = (char)((ushort)uVar2 >> 8);
      pcVar3[2] = '|';
      pcVar3[3] = '\x0f';
      pcVar3[4] = 'd';
      pcVar3[5] = cVar1;
      pcVar3[6] = '\0';
      DAT_ram_e3cf = DAT_ram_e3cf + 1;
      return;
    }
    pcVar3 = pcVar3 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:8f3b FUN_ram_8f3b -- automatic, unverified */

byte FUN_ram_8f3b(undefined2 param_1)

{
  return ~(byte)((ushort)param_1 >> 8);
}



/* ram:8f43 FUN_ram_8f43 -- automatic, unverified */

void FUN_ram_8f43(void)

{
  FUN_ram_8f4f();
  FUN_ram_8a5c();
  FUN_ram_8fd2();
  FUN_ram_8a2a();
  return;
}



/* ram:8f4f FUN_ram_8f4f -- automatic, unverified */

char FUN_ram_8f4f(undefined1 *param_1)

{
  char cVar1;
  char cVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_e200;
  cVar2 = '\b';
  do {
    cVar1 = '\0';
    if (*pcVar3 != '\0') {
      if ((char)(pcVar3[0xc] - pcVar3[3]) == '\0') {
        pcVar3[0xe] = pcVar3[0xe] + '\x01';
      }
      FUN_ram_8fc5(pcVar3[0xc] - pcVar3[3],*(undefined2 *)(pcVar3 + 6));
      pcVar3[6] = (char)param_1;
      pcVar3[7] = (char)((ushort)param_1 >> 8);
      cVar1 = '\0';
      if ((((pcVar3[0xe] & 0x80U) == 0) && (cVar1 = pcVar3[0xd] - pcVar3[5], cVar1 == '\0')) &&
         (1 < (byte)pcVar3[0xe])) {
        pcVar3[0xe] = pcVar3[0xe] | 0x80;
        cVar1 = '\0';
      }
      FUN_ram_8fc5(cVar1,*(undefined2 *)(pcVar3 + 8));
      pcVar3[8] = (char)param_1;
      pcVar3[9] = (char)((ushort)param_1 >> 8);
      param_1 = &DAT_ram_e020;
                    /* WARNING: Subroutine does not return */
      if (DAT_ram_e020 != '\x1b') {
        FUN_ram_4901();
      }
      cVar1 = *pcVar3;
      if (cVar1 != '\x02') {
        cVar1 = (DAT_ram_e003 & 0xc) + 0x7c;
        pcVar3[10] = cVar1;
      }
    }
    pcVar3 = pcVar3 + 0x10;
    cVar2 = cVar2 + -1;
    if (cVar2 == '\0') {
      return cVar1;
    }
  } while( true );
}



/* ram:8fc5 FUN_ram_8fc5 -- automatic, unverified */

char FUN_ram_8fc5(char param_1)

{
  return (param_1 >> 2) * '\x02';
}



/* ram:8fd2 FUN_ram_8fd2 -- automatic, unverified */

void FUN_ram_8fd2(undefined2 param_1)

{
  char cVar1;
  char *pcVar2;
  undefined1 in_stack_00000000;
  
  pcVar2 = &DAT_ram_e200;
  cVar1 = (char)((ushort)param_1 >> 8) + '\x01';
  do {
    if ((((*pcVar2 != '\0') && ((pcVar2[7] & 0x80U) != 0)) &&
        ((byte)((DAT_ram_e3d3 - pcVar2[3]) + 0x18U) < 0x38)) &&
       ((byte)((DAT_ram_e3d5 - pcVar2[5]) + 0x30U) < 0x50)) {
      *pcVar2 = '\0';
      pcVar2[3] = -0x20;
    }
    pcVar2 = pcVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_ram_89ad(CONCAT11(7,in_stack_00000000),0x902d);
  FUN_ram_9049();
  DAT_ram_e323 = 0xf;
  if ((9 < DAT_ram_e3d1) && (DAT_ram_e323 = 10, 0x13 < DAT_ram_e3d1)) {
    DAT_ram_e323 = 7;
  }
  return;
}



/* ram:9049 FUN_ram_9049 -- automatic, unverified */

void FUN_ram_9049(void)

{
  char cVar1;
  char cVar2;
  byte bVar3;
  char cVar4;
  char *pcVar5;
  undefined1 uVar6;
  
  cVar2 = DAT_ram_e3d3;
  pcVar5 = &DAT_ram_e344;
  cVar1 = DAT_ram_e3d5 + -6;
  bVar3 = 0;
  do {
    cVar4 = cVar2 + '(';
    if (DAT_ram_e065 == '\x03') {
      cVar4 = cVar2 + '8';
    }
    *pcVar5 = cVar4;
    if ((3 < DAT_ram_e3b0) && (*pcVar5 = 'H', DAT_ram_e065 == '\x03')) {
      *pcVar5 = 'X';
    }
    cVar4 = (char)pcVar5;
    uVar6 = (undefined1)((ushort)pcVar5 >> 8);
    *(byte *)CONCAT11(uVar6,cVar4 + '\x01') = bVar3 * '\x06' + cVar1;
    *(undefined1 *)CONCAT11(uVar6,cVar4 + '\x02') = 0xf8;
    *(undefined1 *)CONCAT11(uVar6,cVar4 + '\x03') = 1;
    pcVar5 = (char *)CONCAT11(uVar6,cVar4 + '\x04');
    bVar3 = bVar3 + 1;
  } while (bVar3 < 3);
  return;
}



/* ram:9090 state_9090 -- automatic, unverified */

void state_9090(void)

{
  byte bVar1;
  undefined2 uStack_2;
  
  bVar1 = 0;
  if (DAT_ram_e3c0 == '\0') {
    uStack_2 = 0x9099;
    FUN_ram_9120(0);
    if ((bVar1 & 1) == 0) {
      if (DAT_ram_e600 == '\x02') {
        return;
      }
      DAT_ram_e600 = 2;
      DAT_ram_e60b = 0x90;
      FUN_ram_490c(0xae);
      return;
    }
  }
  if ((char)(DAT_ram_e3b0 - 1U) < '\0') {
    return;
  }
  if ((byte)(DAT_ram_e3b0 - 1U) < 4) {
    register0x44 = (BADSPACEBASE *)&uStack_2;
    uStack_2 = 0x9156;
  }
                    /* WARNING: Subroutine does not return */
  *(undefined2 *)((short)register0x44 + -2) = 0x90ac;
  inline_dispatch();
}



/* ram:90bc state_90bc -- automatic, unverified */

void state_90bc(void)

{
                    /* WARNING: Subroutine does not return */
  FUN_ram_940f();
}



/* ram:90bf state_90bf -- automatic, unverified */

void state_90bf(void)

{
  FUN_ram_4412(0x1c20,0xaf73);
  FUN_ram_440c(0xb060);
  FUN_ram_4478(6,0x1ca0,0x1d50);
  FUN_ram_4412(0x1e00,0xabdc);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:90e5 state_90e5 -- automatic, unverified */

byte state_90e5(void)

{
  byte in_F;
  byte bVar1;
  char cVar2;
  undefined1 *puVar3;
  
  FUN_ram_8a8b();
  if (!(bool)(in_F >> 6 & 1)) {
    FUN_ram_89cd();
    FUN_ram_9108();
    if ((in_F >> 6 & 1) != 0) {
      if (DAT_ram_e3e1 != 0) {
        return DAT_ram_e3e1;
      }
      if ((DAT_ram_e003 & 0x1f) != 0) {
        return DAT_ram_e003 & 0x1f;
      }
      bVar1 = FUN_ram_857f(0,&DAT_ram_e3d0);
      return bVar1;
    }
  }
  DAT_ram_e3b0 = 5;
  DAT_ram_e3c0 = 5;
  cVar2 = '\n';
  puVar3 = &DAT_ram_e200;
  do {
    *puVar3 = 0;
    puVar3[3] = 0xe0;
    puVar3 = puVar3 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  FUN_ram_8a2a();
  DAT_ram_e3b1 = 0x40;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x17);
}



/* ram:9108 FUN_ram_9108 -- automatic, unverified */

undefined1 FUN_ram_9108(void)

{
  undefined1 uVar1;
  
  if (DAT_ram_e3e1 == '\0') {
    uVar1 = FUN_ram_8929();
    return uVar1;
  }
  FUN_ram_98e6(DAT_ram_e3e1,0x911c);
  return DAT_ram_e3bf;
}



/* ram:9120 FUN_ram_9120 -- automatic, unverified */

void FUN_ram_9120(void)

{
  byte bVar1;
  
  if (DAT_ram_e3e1 != '\0') {
    FUN_ram_8761(DAT_ram_e3e1,0x1816);
    return;
  }
  bVar1 = (byte)(DAT_ram_e3d5 - 3U) < 9;
  FUN_ram_8761(DAT_ram_e3d5 + -0xc,0xe28,DAT_ram_e3d3 + '\x01');
  if ((bVar1 & 1) == 0) {
    return;
  }
  FUN_ram_8761(DAT_ram_e3d5 + -4,0x100f,DAT_ram_e3d3 + '\x0e');
  return;
}



/* ram:9201 state_9201 -- automatic, unverified */

void state_9201(void)

{
  undefined2 uStack_2;
  
  if ((char)(DAT_ram_e3b0 - 1U) < '\0') {
    return;
  }
  if ((byte)(DAT_ram_e3b0 - 1U) < 4) {
    register0x44 = (BADSPACEBASE *)&uStack_2;
    uStack_2 = 0x9317;
  }
                    /* WARNING: Subroutine does not return */
  *(undefined2 *)((short)register0x44 + -2) = 0x9211;
  inline_dispatch();
}



/* ram:9221 state_9221 -- automatic, unverified */

void state_9221(void)

{
                    /* WARNING: Subroutine does not return */
  FUN_ram_940f();
}



/* ram:9224 state_9224 -- automatic, unverified */

void state_9224(void)

{
  FUN_ram_440c(0xaea6);
  FUN_ram_4412(0x1f80,0xabdc);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:9238 state_9238 -- automatic, unverified */

byte state_9238(void)

{
  byte bVar1;
  
  bVar1 = (DAT_ram_e090 & 0x3f) - 1;
  if (bVar1 != 0) {
    return bVar1;
  }
  DAT_ram_e3d3 = DAT_ram_e3d3 + 8;
  if ((char)DAT_ram_e3d3 < '\0') {
    return DAT_ram_e3d3;
  }
  if (DAT_ram_e3d3 < 0x20) {
    return DAT_ram_e3d3;
  }
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return DAT_ram_e3d3;
}



/* ram:924c state_924c -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_924c(void)

{
  _DAT_ram_e3b3 = 0x208;
  _DAT_ram_e3d8 = 0x280;
  _DAT_ram_e3b1 = 0x1c20;
  _DAT_ram_e3d6 = 0x100;
  _DAT_ram_e3b7 = 0x7820;
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:926e state_926e -- automatic, unverified */

void state_926e(void)

{
  byte in_F;
  char cVar1;
  undefined1 *puVar2;
  
  FUN_ram_8a8b();
  if (!(bool)(in_F >> 6 & 1)) {
    FUN_ram_89cd();
    FUN_ram_9304();
    FUN_ram_8929();
    if ((in_F >> 6 & 1) != 0) {
      if ((DAT_ram_e003 & 0x1f) != 0) {
        return;
      }
      FUN_ram_857f(0,&DAT_ram_e3d0);
      return;
    }
  }
  DAT_ram_e3b0 = 5;
  DAT_ram_e3c0 = 5;
  cVar1 = '\n';
  puVar2 = &DAT_ram_e200;
  do {
    *puVar2 = 0;
    puVar2[3] = 0xe0;
    puVar2 = puVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_ram_8a2a();
  DAT_ram_e3b1 = 0x40;
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x17);
}



/* ram:928f state_928f -- automatic, unverified */

void state_928f(void)

{
  FUN_ram_9401();
  DAT_ram_e3b1 = DAT_ram_e3b1 + -1;
  if (DAT_ram_e3b1 != '\0') {
    return;
  }
  DAT_ram_e3b1 = 0x30;
  FUN_ram_44cd();
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x6b);
}



/* ram:92ac state_92ac -- automatic, unverified */

void state_92ac(void)

{
  if ((DAT_ram_e003 & 8) == 0) {
    FUN_ram_8931();
  }
  else {
    FUN_ram_8979();
  }
  DAT_ram_e3b1 = DAT_ram_e3b1 + -1;
  if (DAT_ram_e3b1 == '\0') {
    DAT_ram_e3b1 = 0x30;
    DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
    return;
  }
  return;
}



/* ram:92c3 state_92c3 -- automatic, unverified */

void state_92c3(void)

{
  if ((DAT_ram_e003 & 8) == 0) {
    FUN_ram_8989();
  }
  else {
    FUN_ram_8979();
  }
  DAT_ram_e3b1 = DAT_ram_e3b1 + -1;
  if (DAT_ram_e3b1 == '\0') {
    DAT_ram_e3b1 = 0x30;
    DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
    return;
  }
  return;
}



/* ram:92d2 state_92d2 -- automatic, unverified */

void state_92d2(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  if ((DAT_ram_e003 & 8) == 0) {
    FUN_ram_8989();
  }
  else {
    FUN_ram_92f6();
  }
  DAT_ram_e3b1 = DAT_ram_e3b1 + -1;
  if (DAT_ram_e3b1 == '\0') {
    puVar3 = &DAT_ram_e3b0;
    puVar2 = &DAT_ram_e3b1;
    sVar1 = 0x3f;
    DAT_ram_e3b0 = 0;
    DAT_ram_e3b1 = 0;
    do {
      *puVar2 = *puVar3;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
      sVar1 = sVar1 + -1;
    } while (sVar1 != 0);
    DAT_ram_e660 = 1;
    sVar1 = 0x27;
    puVar3 = &DAT_ram_e320;
    puVar2 = &DAT_ram_e321;
    DAT_ram_e320 = 0xe0;
    do {
      *puVar2 = *puVar3;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
      sVar1 = sVar1 + -1;
    } while (sVar1 != 0);
    return;
  }
  return;
}



/* ram:92f6 FUN_ram_92f6 -- automatic, unverified */

void FUN_ram_92f6(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  sVar1 = 0x27;
  puVar3 = &DAT_ram_e320;
  puVar2 = &DAT_ram_e321;
  DAT_ram_e320 = 0xe0;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}



/* ram:9304 FUN_ram_9304 -- automatic, unverified */

void FUN_ram_9304(void)

{
  char in_F;
  
  if ((bool)(DAT_ram_e003 & 1)) {
    FUN_ram_8a5c(DAT_ram_e003 >> 1 | in_F << 7);
  }
  FUN_ram_938b();
  FUN_ram_935c();
  FUN_ram_93de();
  FUN_ram_8a2a();
  return;
}



/* ram:935c FUN_ram_935c -- automatic, unverified */

void FUN_ram_935c(void)

{
  char cVar1;
  char cVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_e200;
  cVar2 = '\v';
  do {
    if (*pcVar3 == '\x02') {
      cVar1 = -0x78;
      if ((pcVar3[0xc] & 4U) != 0) {
        cVar1 = -0x74;
      }
      pcVar3[10] = cVar1;
      cVar1 = pcVar3[0xc] + -1;
      pcVar3[0xc] = cVar1;
      if (cVar1 == '\0') {
        *pcVar3 = '\0';
        pcVar3[3] = -0x20;
      }
    }
    pcVar3 = pcVar3 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:938b FUN_ram_938b -- automatic, unverified */

char FUN_ram_938b(void)

{
  char cVar1;
  char cVar2;
  byte bVar3;
  char cVar4;
  char cVar5;
  byte in_R;
  char *pcVar6;
  
  bVar3 = DAT_ram_e604;
  cVar2 = DAT_ram_e602;
  pcVar6 = &DAT_ram_e200;
  cVar5 = '\n';
  do {
    cVar4 = *pcVar6 + -1;
    if (cVar4 == '\0') {
      cVar4 = pcVar6[0xc];
      if (cVar4 == '\0') {
        cVar4 = (pcVar6[3] - cVar2) + '(';
        if (-1 < cVar4) {
          cVar4 = pcVar6[5] - bVar3;
          if ((byte)pcVar6[5] < bVar3) {
            cVar4 = -cVar4;
          }
          cVar4 = (cVar4 >> 1) + (in_R & 0xf) + 8;
          pcVar6[0xc] = cVar4;
        }
      }
      else {
        cVar1 = pcVar6[0xc] + -1;
        pcVar6[0xc] = cVar1;
        if (cVar1 == '\0') {
          *pcVar6 = '\x02';
          pcVar6[1] = '\x0f';
          pcVar6[0xc] = '\x18';
        }
      }
    }
    pcVar6 = pcVar6 + 0x10;
    cVar5 = cVar5 + -1;
  } while (cVar5 != '\0');
  return cVar4;
}



/* ram:93de FUN_ram_93de -- automatic, unverified */

void FUN_ram_93de(void)

{
  char cVar1;
  undefined2 unaff_BC_;
  undefined2 unaff_DE_;
  undefined2 unaff_HL_;
  byte *pbVar2;
  
  if (DAT_ram_e600 == '\x02') {
    return;
  }
  pbVar2 = &DAT_ram_e200;
  cVar1 = '\b';
  do {
    if ((*pbVar2 != 0) && (*pbVar2 < 3)) {
      FUN_ram_8319(unaff_BC_,unaff_HL_,unaff_DE_);
    }
    pbVar2 = pbVar2 + 0x10;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ram:9401 FUN_ram_9401 -- automatic, unverified */

void FUN_ram_9401(void)

{
  FUN_ram_44cd(DAT_ram_e003 & 2);
  return;
}



/* ram:940f FUN_ram_940f -- automatic, unverified */

void FUN_ram_940f(void)

{
  FUN_ram_964e();
  DAT_ram_e3d3 = 0xd8;
  DAT_ram_e3d5 = 0x78;
  return;
}



/* ram:941d state_941d -- automatic, unverified */

void state_941d(void)

{
                    /* WARNING: Subroutine does not return */
  inline_dispatch(DAT_ram_e3b0 + -1);
}



/* ram:9432 state_9432 -- automatic, unverified */

void state_9432(void)

{
  FUN_ram_964e();
  FUN_ram_4412(0x1e00,0xab9d);
  FUN_ram_4478(4,0x1e00,0x1e90);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:9435 state_9435 -- automatic, unverified */

void state_9435(void)

{
  FUN_ram_4412(0x1e00,0xab9d);
  FUN_ram_4478(4,0x1e00,0x1e90);
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:944b state_944b -- automatic, unverified */

void state_944b(void)

{
  if (DAT_ram_e091 != -0x28) {
    return;
  }
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:9454 state_9454 -- automatic, unverified */

void state_9454(void)

{
  byte in_F;
  byte bVar1;
  char cVar2;
  char cVar3;
  char *pcVar4;
  char *pcVar5;
  
  if (!(bool)(DAT_ram_e090 & 1)) {
    FUN_ram_772a(DAT_ram_e090 >> 1 | in_F << 7);
    if ((DAT_ram_e090 == 0xe) && (DAT_ram_e3d0 == '\0')) {
      FUN_ram_94de(0,&DAT_ram_e3d0);
    }
    FUN_ram_957a();
    FUN_ram_94f1(&DAT_ram_e348);
    pcVar5 = &DAT_ram_e100;
    cVar3 = '\x05';
    do {
      pcVar4 = &DAT_ram_94eb;
      FUN_ram_405b(cVar3);
      cVar2 = *pcVar4;
      pcVar5[0x18] = pcVar5[0x18] + '\x01';
      in_F = (pcVar5[0x18] == cVar2) << 6;
      if (pcVar5[0x18] == cVar2) {
        in_F = (*pcVar5 == '\0') << 6;
        if (*pcVar5 == '\0') {
          FUN_ram_94de(0);
          pcVar5[1] = '\t';
          pcVar5[0x16] = -0x40;
        }
      }
      FUN_ram_9541();
      FUN_ram_749a();
      FUN_ram_7452();
      pcVar5 = pcVar5 + 0x20;
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
  }
  bVar1 = in_F & 0xec;
  FUN_ram_8929();
  if ((bVar1 >> 6 & 1) != 0) {
    if ((DAT_ram_e090 & 0x1f) != 0) {
      return;
    }
    FUN_ram_857f(0,&DAT_ram_e3d0);
    return;
  }
  FUN_ram_8904(4);
  DAT_ram_e3d3 = DAT_ram_e3d3 + -0x10;
  DAT_ram_e3d5 = DAT_ram_e3d5 + -0x10;
  return;
}



/* ram:94de FUN_ram_94de -- automatic, unverified */

void FUN_ram_94de(undefined1 *param_1)

{
  *param_1 = 1;
  param_1[3] = 0x20;
  param_1[5] = 0x7e;
  return;
}



/* ram:94f1 FUN_ram_94f1 -- automatic, unverified */

void FUN_ram_94f1(char *param_1,char *param_2)

{
  char cVar1;
  char cVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_ram_7876;
  if (param_2[10] != '\x18') {
    pcVar3 = &DAT_ram_787e;
  }
  cVar2 = '\x02';
  do {
    cVar1 = param_2[3];
    if (*param_2 == '\0') {
      cVar1 = -0x36;
    }
    *param_1 = cVar1 + *pcVar3;
    param_1[1] = param_2[5] + pcVar3[1];
    param_1[2] = pcVar3[2] + -0x40;
    cVar1 = pcVar3[3];
    if ((((cVar1 == '\x05') && (cVar1 = '\x0f', (DAT_ram_e003 & 4) == 0)) &&
        (0x13 < (byte)param_2[1])) && (cVar1 = '\x04', 0x27 < (byte)param_2[1])) {
      cVar1 = '\x06';
    }
    param_1[3] = cVar1;
    pcVar3 = pcVar3 + 4;
    param_1 = param_1 + 4;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:9541 FUN_ram_9541 -- automatic, unverified */

void FUN_ram_9541(char *param_1)

{
  if ((char)(*param_1 + -1) < '\0') {
    return;
  }
                    /* WARNING: Subroutine does not return */
  inline_dispatch(*param_1 + -1);
}



/* ram:9557 state_9557 -- automatic, unverified */

void state_9557(undefined2 param_1,undefined2 param_2,char *param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  
  uVar4 = (undefined1)((ushort)param_2 >> 8);
  uVar3 = (undefined1)param_2;
  uVar2 = (undefined1)((ushort)param_1 >> 8);
  uVar1 = (undefined1)param_1;
  state_7f15();
  if ((byte)param_3[3] < 0x38) {
    return;
  }
  FUN_ram_84e0();
  FUN_ram_83e1();
  *(short *)(param_3 + 6) = CONCAT11(uVar2,uVar1) * 2;
  *(short *)(param_3 + 8) = CONCAT11(uVar4,uVar3) * 2;
  *param_3 = *param_3 + '\x01';
  return;
}



/* ram:957a FUN_ram_957a -- automatic, unverified */

void FUN_ram_957a(char *param_1)

{
  if ((char)(*param_1 + -1) < '\0') {
    return;
  }
                    /* WARNING: Subroutine does not return */
  inline_dispatch(*param_1 + -1);
}



/* ram:9588 state_9588 -- automatic, unverified */

void state_9588(char *param_1)

{
  FUN_ram_7f18();
  if (DAT_ram_e3d3 < 0x38) {
    return;
  }
  FUN_ram_959f();
  *param_1 = *param_1 + '\x01';
  return;
}



/* ram:9598 state_9598 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_9598(short param_1,short param_2,short param_3)

{
  char cVar1;
  byte bVar2;
  ushort uVar3;
  byte bVar4;
  
  FUN_ram_7f18();
  cVar1 = *(char *)(param_3 + 0xe) + -1;
  *(char *)(param_3 + 0xe) = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  FUN_ram_84e0();
  FUN_ram_83e1();
  if (0x13 < DAT_ram_e3d1) {
    bVar4 = (byte)((ushort)param_1 >> 8);
    if (DAT_ram_e3d1 < 0x28) {
      bVar2 = bVar4 >> 1;
      bVar4 = (byte)param_1 >> 1 | bVar4 << 7;
      uVar3 = CONCAT11(bVar2,bVar4);
      if ((bVar2 & 0x40) != 0) {
        uVar3 = CONCAT11(bVar2,bVar4) | 0x8000;
      }
      param_1 = param_1 + uVar3;
      bVar4 = (byte)((ushort)param_2 >> 8);
      bVar2 = bVar4 >> 1;
      bVar4 = (byte)param_2 >> 1 | bVar4 << 7;
      uVar3 = CONCAT11(bVar2,bVar4);
      if ((bVar2 & 0x40) != 0) {
        uVar3 = CONCAT11(bVar2,bVar4) | 0x8000;
      }
      param_2 = param_2 + uVar3;
    }
    else {
      param_1 = param_1 * 2;
      param_2 = param_2 * 2;
    }
  }
  _DAT_ram_e3d6 = param_1;
  _DAT_ram_e3d8 = param_2;
  *(undefined1 *)(param_3 + 0xe) = 0x30;
  return;
}



/* ram:959f FUN_ram_959f -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_959f(short param_1,short param_2,short param_3)

{
  byte bVar1;
  ushort uVar2;
  byte bVar3;
  
  FUN_ram_84e0();
  FUN_ram_83e1();
  if (0x13 < DAT_ram_e3d1) {
    bVar3 = (byte)((ushort)param_1 >> 8);
    if (DAT_ram_e3d1 < 0x28) {
      bVar1 = bVar3 >> 1;
      bVar3 = (byte)param_1 >> 1 | bVar3 << 7;
      uVar2 = CONCAT11(bVar1,bVar3);
      if ((bVar1 & 0x40) != 0) {
        uVar2 = CONCAT11(bVar1,bVar3) | 0x8000;
      }
      param_1 = param_1 + uVar2;
      bVar3 = (byte)((ushort)param_2 >> 8);
      bVar1 = bVar3 >> 1;
      bVar3 = (byte)param_2 >> 1 | bVar3 << 7;
      uVar2 = CONCAT11(bVar1,bVar3);
      if ((bVar1 & 0x40) != 0) {
        uVar2 = CONCAT11(bVar1,bVar3) | 0x8000;
      }
      param_2 = param_2 + uVar2;
    }
    else {
      param_1 = param_1 * 2;
      param_2 = param_2 * 2;
    }
  }
  _DAT_ram_e3d6 = param_1;
  _DAT_ram_e3d8 = param_2;
  *(undefined1 *)(param_3 + 0xe) = 0x30;
  return;
}



/* ram:95e4 state_95e4 -- automatic, unverified */

void state_95e4(void)

{
  undefined2 uStack_2;
  
  uStack_2 = 0x95e7;
  FUN_ram_985c();
  if ((char)(DAT_ram_e3b0 - 1U) < '\0') {
    return;
  }
  if ((byte)(DAT_ram_e3b0 - 1U) < 5) {
    register0x44 = (BADSPACEBASE *)&uStack_2;
    uStack_2 = 0x96cc;
  }
                    /* WARNING: Subroutine does not return */
  *(undefined2 *)((short)register0x44 + -2) = 0x95f7;
  inline_dispatch();
}



/* ram:9609 state_9609 -- automatic, unverified */

void state_9609(void)

{
  undefined1 uVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  
  FUN_ram_964e();
  DAT_ram_e3d3 = 0xd8;
  DAT_ram_e3d5 = 0x68;
  DAT_ram_e3e0 = 3;
  puVar4 = &DAT_ram_e400;
  puVar3 = &DAT_ram_965c;
  cVar2 = '\x06';
  do {
    *puVar4 = 1;
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar4[4] = uVar1;
    puVar4[3] = 0xf;
    puVar4[8] = 0x14;
    puVar4 = puVar4 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  DAT_ram_e400 = 0;
  DAT_ram_e408 = 0x40;
  FUN_ram_440c(0xb124);
  state_8ae6();
  return;
}



/* ram:9645 state_9645 -- automatic, unverified */

void state_9645(void)

{
  FUN_ram_440c(0xb124);
  state_8ae6();
  return;
}



/* ram:964e FUN_ram_964e -- automatic, unverified */

void FUN_ram_964e(void)

{
  short sVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  puVar3 = &DAT_ram_e3d0;
  puVar2 = &DAT_ram_e3d1;
  sVar1 = 0xdf;
  DAT_ram_e3d0 = 0;
  do {
    *puVar2 = *puVar3;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}



/* ram:9662 state_9662 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_9662(void)

{
  FUN_ram_8d87();
  DAT_ram_e3da = DAT_ram_e604 & 0xf8;
  _DAT_ram_e3d8 = 0;
  if ((byte)((DAT_ram_e3d5 & 0xf8) + 0x10) != (DAT_ram_e604 & 0xf8)) {
    _DAT_ram_e3d8 = 0xff00;
  }
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:9687 state_9687 -- automatic, unverified */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void state_9687(void)

{
  short sVar1;
  
  DAT_ram_e3e1 = (DAT_ram_e003 & 4) == 0;
  sVar1 = _DAT_ram_e3d4;
  if (DAT_ram_e3da != (byte)(((byte)(_DAT_ram_e3d4 >> 8) & 0xf8) + 0x10)) {
    _DAT_ram_e3d4 = _DAT_ram_e3d4 + _DAT_ram_e3d8;
    sVar1 = CONCAT11(0x20,(char)_DAT_ram_e3d4);
    if ((0x1fff < _DAT_ram_e3d4) &&
       (sVar1 = CONCAT11(0xb0,(char)_DAT_ram_e3d4), _DAT_ram_e3d4 < 0xb000)) {
      return;
    }
  }
  _DAT_ram_e3d4 = sVar1;
  DAT_ram_e3db = 0x50;
  DAT_ram_e3b0 = DAT_ram_e3b0 + '\x01';
  return;
}



/* ram:96c1 state_96c1 -- automatic, unverified */

void state_96c1(void)

{
  DAT_ram_e3db = DAT_ram_e3db + -1;
  if (DAT_ram_e3db != '\0') {
    return;
  }
  DAT_ram_e3b0 = 3;
  return;
}



/* ram:96f8 state_96f8 -- automatic, unverified */

byte state_96f8(short param_1)

{
  bool bVar1;
  byte bVar2;
  char cVar3;
  undefined1 uVar4;
  byte *pbVar5;
  
  if ((*(byte *)(param_1 + 2) & 0xfb) == 2) {
    pbVar5 = &DAT_ram_e620;
    cVar3 = '\x03';
    bVar1 = false;
    do {
      bVar2 = *pbVar5 >> 7;
      if (!(bool)bVar2) {
        FUN_ram_8797(*pbVar5 << 1 | bVar1,pbVar5);
        if (!(bool)(bVar2 & 1)) {
                    /* WARNING: Subroutine does not return */
          FUN_ram_4901(0xb);
        }
      }
      bVar1 = (byte *)0xffef < pbVar5;
      pbVar5 = pbVar5 + 0x10;
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
  }
  FUN_ram_9817();
  bVar2 = *(byte *)(param_1 + 1);
  uVar4 = 0xf;
  if ((((5 < bVar2) && (uVar4 = 7, 10 < bVar2)) && (uVar4 = 10, 0xf < bVar2)) &&
     (uVar4 = 9, 0x14 < bVar2)) {
    uVar4 = 8;
  }
  *(undefined1 *)(param_1 + 3) = uVar4;
  if (*(char *)(param_1 + 5) != '\0') {
    if ((*(char *)(param_1 + 5) == (char)(*(char *)(param_1 + 4) + -8)) &&
       (bVar2 = *(byte *)(param_1 + 2) & 2 ^ 2, *(byte *)(param_1 + 2) = bVar2, bVar2 == 0)) {
      *(undefined1 *)(param_1 + 5) = 0x20;
    }
    *(char *)(param_1 + 5) = *(char *)(param_1 + 5) + -1;
    bVar2 = *(byte *)(param_1 + 5) & 0xf;
    if (bVar2 != 0) {
      return bVar2;
    }
    bVar2 = *(byte *)(param_1 + 2) ^ 4;
    *(byte *)(param_1 + 2) = bVar2;
    return bVar2;
  }
  *(undefined1 *)(param_1 + 5) = *(undefined1 *)(param_1 + 4);
  bVar2 = *(byte *)(param_1 + 2) ^ 1;
  *(byte *)(param_1 + 2) = bVar2;
  return bVar2;
}



/* ram:97ab state_97ab -- automatic, unverified */

void state_97ab(void)

{
  DAT_ram_e3f0 = DAT_ram_e3f0 + '\x01';
  if (DAT_ram_e3f0 == '\x06') {
    DAT_ram_e3d5 = DAT_ram_e3d5 + '\x18';
    FUN_ram_8904(6);
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_ram_4901(0x4d);
}



/* ram:97d6 state_97d6 -- automatic, unverified */

void state_97d6(char *param_1)

{
  char cVar1;
  
  cVar1 = '\b';
  if ((param_1[5] & 2U) != 0) {
    cVar1 = '\t';
  }
  param_1[2] = cVar1;
  cVar1 = param_1[5] + -1;
  param_1[5] = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  param_1[5] = ' ';
  *param_1 = *param_1 + '\x01';
  return;
}



/* ram:97f0 state_97f0 -- automatic, unverified */

void state_97f0(undefined1 *param_1)

{
  char cVar1;
  undefined1 uVar2;
  
  uVar2 = 9;
  if ((param_1[5] & 2) != 0) {
    uVar2 = 10;
  }
  param_1[2] = uVar2;
  cVar1 = param_1[5] + -1;
  param_1[5] = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  param_1[2] = 0xb;
  param_1[3] = 6;
  *param_1 = 0;
  if (DAT_ram_e3f0 != '\x05') {
    return;
  }
  DAT_ram_e400 = 1;
  return;
}



/* ram:9817 FUN_ram_9817 -- automatic, unverified */

byte FUN_ram_9817(short param_1)

{
  byte bVar1;
  
  bVar1 = *(byte *)(param_1 + 2) & 0xfb;
  if (bVar1 != 2) {
    return bVar1;
  }
  bVar1 = 0x3f;
  if ((1 < DAT_ram_e3f0) && (bVar1 = 0x1f, 3 < DAT_ram_e3f0)) {
    bVar1 = 0xf;
  }
  bVar1 = *(byte *)(param_1 + 5) & bVar1;
  if (bVar1 != 0) {
    return bVar1;
  }
  DAT_ram_e463 = *(char *)(param_1 + 6) + '\t';
  DAT_ram_e465 = *(char *)(param_1 + 7) + '\x06';
  bVar1 = FUN_ram_857f(0xe460,param_1);
  return bVar1;
}



/* ram:985c FUN_ram_985c -- automatic, unverified */

void FUN_ram_985c(void)

{
  bool bVar1;
  char cVar2;
  char cVar3;
  char *pcVar4;
  undefined1 uVar5;
  short unaff_BC_;
  char *unaff_HL_;
  char *pcVar6;
  
  if (DAT_ram_e3b0 < 3) {
    return;
  }
  bVar1 = 5 < DAT_ram_e3b0;
  if (DAT_ram_e3b0 != 6) {
    FUN_ram_8db7();
  }
  if (bVar1) {
    return;
  }
  FUN_ram_8dd1(CONCAT11(DAT_ram_e3d5,DAT_ram_e3d3) & 0xf8ff);
  if ((bool)(DAT_ram_e090 & 1)) {
    return;
  }
  pcVar6 = &DAT_ram_e590;
  cVar3 = '\x03';
  do {
    uVar5 = (undefined1)unaff_BC_;
    pcVar4 = &DAT_ram_8cbf;
    FUN_ram_405b(cVar3);
    cVar2 = *pcVar4;
    pcVar6[0x18] = pcVar6[0x18] + '\x01';
    if ((pcVar6[0x18] == cVar2) && (*pcVar6 == '\0')) {
      *pcVar6 = '\x01';
      pcVar6[1] = '\f';
      pcVar6[3] = '\x10';
      DAT_ram_e666 = DAT_ram_e666 + 1;
      pcVar6[5] = (DAT_ram_e666 & 1) + 0x7f;
    }
    FUN_ram_75ea(pcVar6[1]);
    FUN_ram_749a();
    FUN_ram_7452();
    pcVar4 = &DAT_ram_e338;
    FUN_ram_4060(cVar3 * '\b',unaff_HL_);
    unaff_HL_ = &DAT_ram_7896;
    if (pcVar6[10] != '\x1c') {
      unaff_HL_ = &DAT_ram_789e;
    }
    unaff_BC_ = CONCAT11(2,uVar5);
    do {
      cVar2 = pcVar6[3];
      if (*pcVar6 == '\0') {
        cVar2 = -0x36;
      }
      *pcVar4 = cVar2 + *unaff_HL_;
      pcVar4[1] = pcVar6[5] + unaff_HL_[1];
      pcVar4[2] = unaff_HL_[2] + -0x40;
      pcVar4[3] = unaff_HL_[3];
      pcVar4 = pcVar4 + 4;
      unaff_HL_ = unaff_HL_ + 4;
      cVar2 = (char)((ushort)(unaff_BC_ + -1) >> 8) + -1;
      unaff_BC_ = CONCAT11(cVar2,(char)(unaff_BC_ + -1));
    } while (cVar2 != '\0');
    pcVar6 = pcVar6 + 0x20;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return;
}



/* ram:98db FUN_ram_98db -- automatic, unverified */

void FUN_ram_98db(void)

{
  short sVar1;
  char cVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  char *pcVar5;
  byte *pbVar6;
  
  puVar4 = (undefined1 *)0x9999;
  FUN_ram_405b(DAT_ram_e065 << 2);
  puVar3 = &DAT_ram_e3bb;
  sVar1 = 4;
  do {
    *puVar3 = *puVar4;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  pbVar6 = &DAT_ram_e620;
  cVar2 = '\x03';
  do {
    if ((*pbVar6 & 0x80) == 0) {
      pcVar5 = (char *)0x99b5;
      FUN_ram_405b(pbVar6[5] * '\x02');
      if ((byte)((DAT_ram_e3bb + *pcVar5) - (DAT_ram_e3d5 - pbVar6[4])) <
          (byte)(DAT_ram_e3bc + DAT_ram_e3bb + *pcVar5)) {
        if ((byte)(DAT_ram_e3bd - (DAT_ram_e3d3 - pbVar6[2])) < DAT_ram_e3be) {
          FUN_ram_9937();
        }
      }
    }
    pbVar6 = pbVar6 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:98e6 FUN_ram_98e6 -- automatic, unverified */

void FUN_ram_98e6(undefined1 *param_1)

{
  short sVar1;
  char cVar2;
  undefined1 *puVar3;
  char *pcVar4;
  byte *pbVar5;
  
  puVar3 = &DAT_ram_e3bb;
  sVar1 = 4;
  do {
    *puVar3 = *param_1;
    puVar3 = puVar3 + 1;
    param_1 = param_1 + 1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  pbVar5 = &DAT_ram_e620;
  cVar2 = '\x03';
  do {
    if ((*pbVar5 & 0x80) == 0) {
      pcVar4 = (char *)0x99b5;
      FUN_ram_405b(pbVar5[5] * '\x02');
      if ((byte)((DAT_ram_e3bb + *pcVar4) - (DAT_ram_e3d5 - pbVar5[4])) <
          (byte)(DAT_ram_e3bc + DAT_ram_e3bb + *pcVar4)) {
        if ((byte)(DAT_ram_e3bd - (DAT_ram_e3d3 - pbVar5[2])) < DAT_ram_e3be) {
          FUN_ram_9937();
        }
      }
    }
    pbVar5 = pbVar5 + 0x10;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ram:9937 FUN_ram_9937 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0x491e) */
/* WARNING: Removing unreachable block (ram,0x4930) */
/* WARNING: Removing unreachable block (ram,0x4937) */
/* WARNING: Removing unreachable block (ram,0x4901) */
/* WARNING: Removing unreachable block (ram,0x490a) */
/* WARNING: Removing unreachable block (ram,0x492b) */
/* WARNING: Removing unreachable block (ram,0x493a) */
/* WARNING: Removing unreachable block (ram,0x493f) */
/* WARNING: Removing unreachable block (ram,0x4925) */
/* WARNING: Removing unreachable block (ram,0x4946) */
/* WARNING: Removing unreachable block (ram,0x494e) */
/* WARNING: Removing unreachable block (ram,0x4958) */
/* WARNING: Removing unreachable block (ram,0x4973) */
/* WARNING: Removing unreachable block (ram,0x4976) */

char FUN_ram_9937(byte *param_1)

{
  param_1[2] = 0xe0;
  *param_1 = *param_1 | 0x80;
  if (DAT_ram_e3bf == '\0') {
    FUN_ram_997b();
    FUN_ram_9986();
                    /* WARNING: Subroutine does not return */
    FUN_ram_4901(0xb);
  }
  return DAT_ram_e3bf;
}



/* ram:997b FUN_ram_997b -- automatic, unverified */

char FUN_ram_997b(void)

{
  if (DAT_ram_e065 != '\x02') {
    return DAT_ram_e065;
  }
  return DAT_ram_e3e1 + -1;
}



/* ram:9986 FUN_ram_9986 -- automatic, unverified */

char FUN_ram_9986(void)

{
  if (DAT_ram_e065 != '\x04') {
    return DAT_ram_e065;
  }
  return DAT_ram_e3e2;
}



/* ram:a0a1 FUN_ram_a0a1 -- automatic, unverified */

/* WARNING: Removing unreachable block (ram,0xa220) */
/* WARNING: Removing unreachable block (ram,0xa22e) */
/* WARNING: Removing unreachable block (ram,0xa248) */
/* WARNING: Removing unreachable block (ram,0xa29e) */

void FUN_ram_a0a1(char *param_1,undefined1 *param_2)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  
  do {
    pbVar3 = (byte *)(param_1 + (short)(param_2 + 0x301));
    bVar1 = *pbVar3;
    *pbVar3 = 0xbd;
    param_2 = &DAT_ram_bd01;
    param_1 = &DAT_ram_bd01 + CONCAT11((char)pbVar3,(char)pbVar3);
  } while ((bVar1 & 1) != 0);
  do {
    cVar2 = (char)(param_1 + (short)param_2);
    param_1[(short)param_2] = cVar2;
    param_1 = (char *)CONCAT11((char)param_2,cVar2);
    *param_1 = *param_1 + -1;
    param_2 = (undefined1 *)CONCAT11((char)((ushort)param_2 >> 8),2);
    param_1 = param_1 + -0x64ff;
  } while( true );
}



/* ram:b011 FUN_ram_b011 -- automatic, unverified */

/* WARNING: Instruction at (ram,0xb139) overlaps instruction at (ram,0xb138)
    */
/* WARNING: Removing unreachable block (ram,0xb0cf) */
/* WARNING: Removing unreachable block (ram,0xb01b) */
/* WARNING: Removing unreachable block (ram,0xb01a) */
/* WARNING: Removing unreachable block (ram,0xb01e) */
/* WARNING: Removing unreachable block (ram,0xb026) */
/* WARNING: Removing unreachable block (ram,0xb034) */
/* WARNING: Removing unreachable block (ram,0xb035) */
/* WARNING: Removing unreachable block (ram,0xb036) */
/* WARNING: Removing unreachable block (ram,0xb037) */
/* WARNING: Removing unreachable block (ram,0xb03a) */
/* WARNING: Removing unreachable block (ram,0xb04e) */
/* WARNING: Removing unreachable block (ram,0xb054) */
/* WARNING: Removing unreachable block (ram,0xb055) */
/* WARNING: Removing unreachable block (ram,0xb056) */
/* WARNING: Removing unreachable block (ram,0xb059) */
/* WARNING: Removing unreachable block (ram,0xb05a) */
/* WARNING: Removing unreachable block (ram,0xb05b) */
/* WARNING: Removing unreachable block (ram,0xb060) */
/* WARNING: Removing unreachable block (ram,0xb06f) */
/* WARNING: Removing unreachable block (ram,0xb09d) */
/* WARNING: Removing unreachable block (ram,0xb09e) */
/* WARNING: Removing unreachable block (ram,0xb09f) */
/* WARNING: Removing unreachable block (ram,0xb07d) */
/* WARNING: Removing unreachable block (ram,0xb07e) */
/* WARNING: Removing unreachable block (ram,0xb07f) */
/* WARNING: Removing unreachable block (ram,0xb080) */
/* WARNING: Removing unreachable block (ram,0xb0a0) */
/* WARNING: Removing unreachable block (ram,0xb0a3) */
/* WARNING: Removing unreachable block (ram,0xb0ad) */
/* WARNING: Removing unreachable block (ram,0xb0ae) */
/* WARNING: Removing unreachable block (ram,0xb0b2) */
/* WARNING: Removing unreachable block (ram,0xb0b4) */
/* WARNING: Removing unreachable block (ram,0xb0b7) */
/* WARNING: Removing unreachable block (ram,0xb0d5) */
/* WARNING: Removing unreachable block (ram,0xb0d8) */
/* WARNING: Removing unreachable block (ram,0xb0de) */
/* WARNING: Removing unreachable block (ram,0xb0e9) */
/* WARNING: Removing unreachable block (ram,0xb0ee) */
/* WARNING: Removing unreachable block (ram,0xb0f1) */
/* WARNING: Removing unreachable block (ram,0xb0f4) */
/* WARNING: Removing unreachable block (ram,0xb0fe) */
/* WARNING: Removing unreachable block (ram,0xb138) */
/* WARNING: Removing unreachable block (ram,0xb13b) */
/* WARNING: Removing unreachable block (ram,0xb13c) */
/* WARNING: Removing unreachable block (ram,0xb14a) */
/* WARNING: Removing unreachable block (ram,0xb14b) */
/* WARNING: Removing unreachable block (ram,0xb133) */
/* WARNING: Removing unreachable block (ram,0xb134) */
/* WARNING: Removing unreachable block (ram,0xb135) */
/* WARNING: Removing unreachable block (ram,0xb136) */

void FUN_ram_b011(undefined2 param_1)

{
  undefined2 unaff_AF_;
  
  if ((char)((ushort)unaff_AF_ >> 8) == '\0' && (char)((ushort)param_1 >> 8) == '\x01') {
    return;
  }
  return;
}



/* ram:b300 FUN_ram_b300 -- automatic, unverified */

void FUN_ram_b300(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


