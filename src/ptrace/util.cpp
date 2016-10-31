/*
*  Author: HIT CS HDMC team.
*  Last modified by zhaozhilong: 2010-1-14
*/
#include "util.h"
#include "common.h"
#include <asm/ptrace_offsets.h>	//define PT_F32,etc

long name2offset(const char* name)
{
	long offset;
	if(strcmp(name, "null") == 0)
	{
		offset = NULL_POS;
	}
	else if(strcmp(name, "PT_F32") == 0)
	{
		offset = PT_F32;
	}
	else if(strcmp(name, "PT_F33") == 0)
	{
		offset = PT_F33;
	}
	else if(strcmp(name, "PT_F34") == 0)
	{
		offset = PT_F34;
	}
	else if(strcmp(name, "PT_F35") == 0)
	{
		offset = PT_F35;
	}
	else if(strcmp(name, "PT_F36") == 0)
	{
		offset = PT_F36;
	}
	else if(strcmp(name, "PT_F37") == 0)
	{
		offset = PT_F37;
	}
	else if(strcmp(name, "PT_F38") == 0)
	{
		offset = PT_F38;
	}
	else if(strcmp(name, "PT_F39") == 0)
	{
		offset = PT_F39;
	}
	else if(strcmp(name, "PT_F40") == 0)
	{
		offset = PT_F40;
	}
	else if(strcmp(name, "PT_F41") == 0)
	{
		offset = PT_F41;
	}
	else if(strcmp(name, "PT_F42") == 0)
	{
		offset = PT_F42;
	}
	else if(strcmp(name, "PT_F43") == 0)
	{
		offset = PT_F43;
	}
	else if(strcmp(name, "PT_F44") == 0)
	{
		offset = PT_F44;
	}
	else if(strcmp(name, "PT_F45") == 0)
	{
		offset = PT_F45;
	}
	else if(strcmp(name, "PT_F46") == 0)
	{
		offset = PT_F46;
	}
	else if(strcmp(name, "PT_F47") == 0)
	{
		offset = PT_F47;
	}
	else if(strcmp(name, "PT_F48") == 0)
	{
		offset = PT_F48;
	}
	else if(strcmp(name, "PT_F49") == 0)
	{
		offset = PT_F49;
	}
	else if(strcmp(name, "PT_F50") == 0)
	{
		offset = PT_F50;
	}
	else if(strcmp(name, "PT_F51") == 0)
	{
		offset = PT_F51;
	}
	else if(strcmp(name, "PT_F52") == 0)
	{
		offset = PT_F52;
	}
	else if(strcmp(name, "PT_F53") == 0)
	{
		offset = PT_F53;
	}
	else if(strcmp(name, "PT_F54") == 0)
	{
		offset = PT_F54;
	}
	else if(strcmp(name, "PT_F55") == 0)
	{
		offset = PT_F55;
	}
	else if(strcmp(name, "PT_F56") == 0)
	{
		offset = PT_F56;
	}
	else if(strcmp(name, "PT_F57") == 0)
	{
		offset = PT_F57;
	}
	else if(strcmp(name, "PT_F58") == 0)
	{
		offset = PT_F58;
	}
	else if(strcmp(name, "PT_F59") == 0)
	{
		offset = PT_F59;
	}
	else if(strcmp(name, "PT_F60") == 0)
	{
		offset = PT_F60;
	}
	else if(strcmp(name, "PT_F61") == 0)
	{
		offset = PT_F61;
	}
	else if(strcmp(name, "PT_F62") == 0)
	{
		offset = PT_F62;
	}
	else if(strcmp(name, "PT_F63") == 0)
	{
		offset = PT_F63;
	}
	else if(strcmp(name, "PT_F64") == 0)
	{
		offset = PT_F64;
	}
	else if(strcmp(name, "PT_F65") == 0)
	{
		offset = PT_F65;
	}
	else if(strcmp(name, "PT_F66") == 0)
	{
		offset = PT_F66;
	}
	else if(strcmp(name, "PT_F67") == 0)
	{
		offset = PT_F67;
	}
	else if(strcmp(name, "PT_F68") == 0)
	{
		offset = PT_F68;
	}
	else if(strcmp(name, "PT_F69") == 0)
	{
		offset = PT_F69;
	}
	else if(strcmp(name, "PT_F70") == 0)
	{
		offset = PT_F70;
	}
	else if(strcmp(name, "PT_F71") == 0)
	{
		offset = PT_F71;
	}
	else if(strcmp(name, "PT_F72") == 0)
	{
		offset = PT_F72;
	}
	else if(strcmp(name, "PT_F73") == 0)
	{
		offset = PT_F73;
	}
	else if(strcmp(name, "PT_F74") == 0)
	{
		offset = PT_F74;
	}
	else if(strcmp(name, "PT_F75") == 0)
	{
		offset = PT_F75;
	}
	else if(strcmp(name, "PT_F76") == 0)
	{
		offset = PT_F76;
	}
	else if(strcmp(name, "PT_F77") == 0)
	{
		offset = PT_F77;
	}
	else if(strcmp(name, "PT_F78") == 0)
	{
		offset = PT_F78;
	}
	else if(strcmp(name, "PT_F79") == 0)
	{
		offset = PT_F79;
	}
	else if(strcmp(name, "PT_F80") == 0)
	{
		offset = PT_F80;
	}
	else if(strcmp(name, "PT_F81") == 0)
	{
		offset = PT_F81;
	}
	else if(strcmp(name, "PT_F82") == 0)
	{
		offset = PT_F82;
	}
	else if(strcmp(name, "PT_F83") == 0)
	{
		offset = PT_F83;
	}
	else if(strcmp(name, "PT_F84") == 0)
	{
		offset = PT_F84;
	}
	else if(strcmp(name, "PT_F85") == 0)
	{
		offset = PT_F85;
	}
	else if(strcmp(name, "PT_F86") == 0)
	{
		offset = PT_F86;
	}
	else if(strcmp(name, "PT_F87") == 0)
	{
		offset = PT_F87;
	}
	else if(strcmp(name, "PT_F88") == 0)
	{
		offset = PT_F88;
	}
	else if(strcmp(name, "PT_F89") == 0)
	{
		offset = PT_F89;
	}
	else if(strcmp(name, "PT_F90") == 0)
	{
		offset = PT_F90;
	}
	else if(strcmp(name, "PT_F91") == 0)
	{
		offset = PT_F91;
	}
	else if(strcmp(name, "PT_F92") == 0)
	{
		offset = PT_F92;
	}
	else if(strcmp(name, "PT_F93") == 0)
	{
		offset = PT_F93;
	}
	else if(strcmp(name, "PT_F94") == 0)
	{
		offset = PT_F94;
	}
	else if(strcmp(name, "PT_F95") == 0)
	{
		offset = PT_F95;
	}
	else if(strcmp(name, "PT_F96") == 0)
	{
		offset = PT_F96;
	}
	else if(strcmp(name, "PT_F97") == 0)
	{
		offset = PT_F97;
	}
	else if(strcmp(name, "PT_F98") == 0)
	{
		offset = PT_F98;
	}
	else if(strcmp(name, "PT_F99") == 0)
	{
		offset = PT_F99;
	}
	else if(strcmp(name, "PT_F100") == 0)
	{
		offset = PT_F100;
	}
	else if(strcmp(name, "PT_F101") == 0)
	{
		offset = PT_F101;
	}
	else if(strcmp(name, "PT_F102") == 0)
	{
		offset = PT_F102;
	}
	else if(strcmp(name, "PT_F103") == 0)
	{
		offset = PT_F103;
	}
	else if(strcmp(name, "PT_F104") == 0)
	{
		offset = PT_F104;
	}
	else if(strcmp(name, "PT_F105") == 0)
	{
		offset = PT_F105;
	}
	else if(strcmp(name, "PT_F106") == 0)
	{
		offset = PT_F106;
	}
	else if(strcmp(name, "PT_F107") == 0)
	{
		offset = PT_F107;
	}
	else if(strcmp(name, "PT_F108") == 0)
	{
		offset = PT_F108;
	}
	else if(strcmp(name, "PT_F109") == 0)
	{
		offset = PT_F109;
	}
	else if(strcmp(name, "PT_F110") == 0)
	{
		offset = PT_F110;
	}
	else if(strcmp(name, "PT_F111") == 0)
	{
		offset = PT_F111;
	}
	else if(strcmp(name, "PT_F112") == 0)
	{
		offset = PT_F112;
	}
	else if(strcmp(name, "PT_F113") == 0)
	{
		offset = PT_F113;
	}
	else if(strcmp(name, "PT_F114") == 0)
	{
		offset = PT_F114;
	}
	else if(strcmp(name, "PT_F115") == 0)
	{
		offset = PT_F115;
	}
	else if(strcmp(name, "PT_F116") == 0)
	{
		offset = PT_F116;
	}
	else if(strcmp(name, "PT_F117") == 0)
	{
		offset = PT_F117;
	}
	else if(strcmp(name, "PT_F118") == 0)
	{
		offset = PT_F118;
	}
	else if(strcmp(name, "PT_F119") == 0)
	{
		offset = PT_F119;
	}
	else if(strcmp(name, "PT_F120") == 0)
	{
		offset = PT_F120;
	}
	else if(strcmp(name, "PT_F121") == 0)
	{
		offset = PT_F121;
	}
	else if(strcmp(name, "PT_F122") == 0)
	{
		offset = PT_F122;
	}
	else if(strcmp(name, "PT_F123") == 0)
	{
		offset = PT_F123;
	}
	else if(strcmp(name, "PT_F124") == 0)
	{
		offset = PT_F124;
	}
	else if(strcmp(name, "PT_F125") == 0)
	{
		offset = PT_F125;
	}
	else if(strcmp(name, "PT_F126") == 0)
	{
		offset = PT_F126;
	}
	else if(strcmp(name, "PT_F127") == 0)
	{
		offset = PT_F127;
	}
	else if(strcmp(name, "PT_NAT_BITS") == 0)
	{
		offset = PT_NAT_BITS;
	}
	else if(strcmp(name, "PT_F2") == 0)
	{
		offset = PT_F2;
	}
	else if(strcmp(name, "PT_F3") == 0)
	{
		offset = PT_F3;
	}
	else if(strcmp(name, "PT_F4") == 0)
	{
		offset = PT_F4;
	}
	else if(strcmp(name, "PT_F5") == 0)
	{
		offset = PT_F5;
	}
	else if(strcmp(name, "PT_F10") == 0)
	{
		offset = PT_F10;
	}
	else if(strcmp(name, "PT_F11") == 0)
	{
		offset = PT_F11;
	}
	else if(strcmp(name, "PT_F12") == 0)
	{
		offset = PT_F12;
	}
	else if(strcmp(name, "PT_F13") == 0)
	{
		offset = PT_F13;
	}
	else if(strcmp(name, "PT_F14") == 0)
	{
		offset = PT_F14;
	}
	else if(strcmp(name, "PT_F15") == 0)
	{
		offset = PT_F15;
	}
	else if(strcmp(name, "PT_F16") == 0)
	{
		offset = PT_F16;
	}
	else if(strcmp(name, "PT_F17") == 0)
	{
		offset = PT_F17;
	}
	else if(strcmp(name, "PT_F18") == 0)
	{
		offset = PT_F18;
	}
	else if(strcmp(name, "PT_F19") == 0)
	{
		offset = PT_F19;
	}
	else if(strcmp(name, "PT_F20") == 0)
	{
		offset = PT_F20;
	}
	else if(strcmp(name, "PT_F21") == 0)
	{
		offset = PT_F21;
	}
	else if(strcmp(name, "PT_F22") == 0)
	{
		offset = PT_F22;
	}
	else if(strcmp(name, "PT_F23") == 0)
	{
		offset = PT_F23;
	}
	else if(strcmp(name, "PT_F24") == 0)
	{
		offset = PT_F24;
	}
	else if(strcmp(name, "PT_F25") == 0)
	{
		offset = PT_F25;
	}
	else if(strcmp(name, "PT_F26") == 0)
	{
		offset = PT_F26;
	}
	else if(strcmp(name, "PT_F27") == 0)
	{
		offset = PT_F27;
	}
	else if(strcmp(name, "PT_F28") == 0)
	{
		offset = PT_F28;
	}
	else if(strcmp(name, "PT_F29") == 0)
	{
		offset = PT_F29;
	}
	else if(strcmp(name, "PT_F30") == 0)
	{
		offset = PT_F30;
	}
	else if(strcmp(name, "PT_F31") == 0)
	{
		offset = PT_F31;
	}
	else if(strcmp(name, "PT_R4") == 0)
	{
		offset = PT_R4;
	}
	else if(strcmp(name, "PT_R5") == 0)
	{
		offset = PT_R5;
	}
	else if(strcmp(name, "PT_R6") == 0)
	{
		offset = PT_R6;
	}
	else if(strcmp(name, "PT_R7") == 0)
	{
		offset = PT_R7;
	}
	else if(strcmp(name, "PT_B1") == 0)
	{
		offset = PT_B1;
	}
	else if(strcmp(name, "PT_B2") == 0)
	{
		offset = PT_B2;
	}
	else if(strcmp(name, "PT_B3") == 0)
	{
		offset = PT_B3;
	}
	else if(strcmp(name, "PT_B4") == 0)
	{
		offset = PT_B4;
	}
	else if(strcmp(name, "PT_B5") == 0)
	{
		offset = PT_B5;
	}
	else if(strcmp(name, "PT_AR_EC") == 0)
	{
		offset = PT_AR_EC;
	}
	else if(strcmp(name, "PT_AR_LC") == 0)
	{
		offset = PT_AR_LC;
	}
	else if(strcmp(name, "PT_CR_IPSR") == 0)
	{
		offset = PT_CR_IPSR;
	}
	else if(strcmp(name, "PT_CR_IIP") == 0)
	{
		offset = PT_CR_IIP;
	}
	else if(strcmp(name, "PT_CFM") == 0)
	{
		offset = PT_CFM;
	}
	else if(strcmp(name, "PT_AR_UNAT") == 0)
	{
		offset = PT_AR_UNAT;
	}
	else if(strcmp(name, "PT_AR_PFS") == 0)
	{
		offset = PT_AR_PFS;
	}
	else if(strcmp(name, "PT_AR_RSC") == 0)
	{
		offset = PT_AR_RSC;
	}
	else if(strcmp(name, "PT_AR_RNAT") == 0)
	{
		offset = PT_AR_RNAT;
	}
	else if(strcmp(name, "PT_AR_BSPSTORE") == 0)
	{
		offset = PT_AR_BSPSTORE;
	}
	else if(strcmp(name, "PT_PR") == 0)
	{
		offset = PT_PR;
	}
	else if(strcmp(name, "PT_B6") == 0)
	{
		offset = PT_B6;
	}
	else if(strcmp(name, "PT_AR_BSP") == 0)
	{
		offset = PT_AR_BSP;
	}
	else if(strcmp(name, "PT_R1") == 0)
	{
		offset = PT_R1;
	}
	else if(strcmp(name, "PT_R2") == 0)
	{
		offset = PT_R2;
	}
	else if(strcmp(name, "PT_R3") == 0)
	{
		offset = PT_R3;
	}
	else if(strcmp(name, "PT_R12") == 0)
	{
		offset = PT_R12;
	}
	else if(strcmp(name, "PT_R13") == 0)
	{
		offset = PT_R13;
	}
	else if(strcmp(name, "PT_R14") == 0)
	{
		offset = PT_R14;
	}
	else if(strcmp(name, "PT_R15") == 0)
	{
		offset = PT_R15;
	}
	else if(strcmp(name, "PT_R8") == 0)
	{
		offset = PT_R8;
	}
	else if(strcmp(name, "PT_R9") == 0)
	{
		offset = PT_R9;
	}
	else if(strcmp(name, "PT_R10") == 0)
	{
		offset = PT_R10;
	}
	else if(strcmp(name, "PT_R11") == 0)
	{
		offset = PT_R11;
	}
	else if(strcmp(name, "PT_R16") == 0)
	{
		offset = PT_R16;
	}
	else if(strcmp(name, "PT_R17") == 0)
	{
		offset = PT_R17;
	}
	else if(strcmp(name, "PT_R18") == 0)
	{
		offset = PT_R18;
	}
	else if(strcmp(name, "PT_R19") == 0)
	{
		offset = PT_R19;
	}
	else if(strcmp(name, "PT_R20") == 0)
	{
		offset = PT_R20;
	}
	else if(strcmp(name, "PT_R21") == 0)
	{
		offset = PT_R21;
	}
	else if(strcmp(name, "PT_R22") == 0)
	{
		offset = PT_R22;
	}
	else if(strcmp(name, "PT_R23") == 0)
	{
		offset = PT_R23;
	}
	else if(strcmp(name, "PT_R24") == 0)
	{
		offset = PT_R24;
	}
	else if(strcmp(name, "PT_R25") == 0)
	{
		offset = PT_R25;
	}
	else if(strcmp(name, "PT_R26") == 0)
	{
		offset = PT_R26;
	}
	else if(strcmp(name, "PT_R27") == 0)
	{
		offset = PT_R27;
	}
	else if(strcmp(name, "PT_R28") == 0)
	{
		offset = PT_R28;
	}
	else if(strcmp(name, "PT_R29") == 0){
		offset = PT_R29;
	}
	else if(strcmp(name, "PT_R30") == 0)
	{
		offset = PT_R30;
	}
	else if(strcmp(name, "PT_R31") == 0)
	{
		offset = PT_R31;
	}
	else if(strcmp(name, "PT_AR_CCV") == 0)
	{
		offset = PT_AR_CCV;
	}
	else if(strcmp(name, "PT_AR_FPSR") == 0)
	{
		offset = PT_AR_FPSR;
	}
	else if(strcmp(name, "PT_B0") == 0)
	{
		offset = PT_B0;
	}
	else if(strcmp(name, "PT_B7") == 0)
	{
		offset = PT_B7;
	}
	else if(strcmp(name, "PT_F6") == 0)
	{
		offset = PT_F6;
	}
	else if(strcmp(name, "PT_F7") == 0)
	{
		offset = PT_F7;
	}
	else if(strcmp(name, "PT_F8") == 0)
	{
		offset = PT_F8;
	}
	else if(strcmp(name, "PT_F9") == 0)
	{
		offset = PT_F9;
	}
	else if(strcmp(name, "PT_AR_CSD") == 0)
	{
		offset = PT_AR_CSD;
	}
	else if(strcmp(name, "PT_AR_SSD") == 0)
	{
		offset = PT_AR_SSD;
	}
	else if(strcmp(name, "PT_DBR") == 0)
	{
		offset = PT_DBR;
	}
	else if(strcmp(name, "PT_IBR") == 0)
	{
		offset = PT_IBR;
	}
	else if(strcmp(name, "PT_PMD") == 0)
	{
		offset = PT_PMD;
	}
	else
	{
		offset = ERR_POS;
	}

	return offset;
}

char * offset2name(const long offset)
{
	switch(offset)
	{
		case  NULL_POS:
			return "NULL_POS";
		case  PT_F32:
			return "PT_F32";
		case  PT_F33:
			return "PT_F33";
		case  PT_F34:
			return "PT_F34";
		case  PT_F35:
			return "PT_F35";
		case  PT_F36:
			return "PT_F36";
		case  PT_F37:
			return "PT_F37";
		case  PT_F38:
			return "PT_F38";
		case  PT_F39:
			return "PT_F39";
		case  PT_F40:
			return "PT_F40";
		case  PT_F41:
			return "PT_F41";
		case  PT_F42:
			return "PT_F42";
		case  PT_F43:
			return "PT_F43";
		case  PT_F44:
			return "PT_F44";
		case  PT_F45:
			return "PT_F45";
		case  PT_F46:
			return "PT_F46";
		case  PT_F47:
			return "PT_F47";
		case  PT_F48:
			return "PT_F48";
		case  PT_F49:
			return "PT_F49";
		case  PT_F50:
			return "PT_F50";
		case  PT_F51:
			return "PT_F51";
		case  PT_F52:
			return "PT_F52";
		case  PT_F53:
			return "PT_F53";
		case  PT_F54:
			return "PT_F54";
		case  PT_F55:
			return "PT_F55";
		case  PT_F56:
			return "PT_F56";
		case  PT_F57:
			return "PT_F57";
		case  PT_F58:
			return "PT_F58";
		case  PT_F59:
			return "PT_F59";
		case  PT_F60:
			return "PT_F60";
		case  PT_F61:
			return "PT_F61";
		case  PT_F62:
			return "PT_F62";
		case  PT_F63:
			return "PT_F63";
		case  PT_F64:
			return "PT_F64";
		case  PT_F65:
			return "PT_F65";
		case  PT_F66:
			return "PT_F66";
		case  PT_F67:
			return "PT_F67";
		case  PT_F68:
			return "PT_F68";
		case  PT_F69:
			return "PT_F69";
		case  PT_F70:
			return "PT_F70";
		case  PT_F71:
			return "PT_F71";
		case  PT_F72:
			return "PT_F72";
		case  PT_F73:
			return "PT_F73";
		case  PT_F74:
			return "PT_F74";
		case  PT_F75:
			return "PT_F75";
		case  PT_F76:
			return "PT_F76";
		case  PT_F77:
			return "PT_F77";
		case  PT_F78:
			return "PT_F78";
		case  PT_F79:
			return "PT_F79";
		case  PT_F80:
			return "PT_F80";
		case  PT_F81:
			return "PT_F81";
		case  PT_F82:
			return "PT_F82";
		case  PT_F83:
			return "PT_F83";
		case  PT_F84:
			return "PT_F84";
		case  PT_F85:
			return "PT_F85";
		case  PT_F86:
			return "PT_F86";
		case  PT_F87:
			return "PT_F87";
		case  PT_F88:
			return "PT_F88";
		case  PT_F89:
			return "PT_F89";
		case  PT_F90:
			return "PT_F90";
		case  PT_F91:
			return "PT_F91";
		case  PT_F92:
			return "PT_F92";
		case  PT_F93:
			return "PT_F93";
		case  PT_F94:
			return "PT_F94";
		case  PT_F95:
			return "PT_F95";
		case  PT_F96:
			return "PT_F96";
		case  PT_F97:
			return "PT_F97";
		case  PT_F98:
			return "PT_F98";
		case  PT_F99:
			return "PT_F99";
		case  PT_F100:
			return "PT_F100";
		case  PT_F101:
			return "PT_F101";
		case  PT_F102:
			return "PT_F102";
		case  PT_F103:
			return "PT_F103";
		case  PT_F104:
			return "PT_F104";
		case  PT_F105:
			return "PT_F105";
		case  PT_F106:
			return "PT_F106";
		case  PT_F107:
			return "PT_F107";
		case  PT_F108:
			return "PT_F108";
		case  PT_F109:
			return "PT_F109";
		case  PT_F110:
			return "PT_F110";
		case  PT_F111:
			return "PT_F111";
		case  PT_F112:
			return "PT_F112";
		case  PT_F113:
			return "PT_F113";
		case  PT_F114:
			return "PT_F114";
		case  PT_F115:
			return "PT_F115";
		case  PT_F116:
			return "PT_F116";
		case  PT_F117:
			return "PT_F117";
		case  PT_F118:
			return "PT_F118";
		case  PT_F119:
			return "PT_F119";
		case  PT_F120:
			return "PT_F120";
		case  PT_F121:
			return "PT_F121";
		case  PT_F122:
			return "PT_F122";
		case  PT_F123:
			return "PT_F123";
		case  PT_F124:
			return "PT_F124";
		case  PT_F125:
			return "PT_F125";
		case  PT_F126:
			return "PT_F126";
		case  PT_F127:
			return "PT_F127";
		case  PT_NAT_BITS:
			return "PT_NAT_BITS";
		case  PT_F2:
			return "PT_F2";
		case  PT_F3:
			return "PT_F3";
		case  PT_F4:
			return "PT_F4";
		case  PT_F5:
			return "PT_F5";
		case  PT_F10:
			return "PT_F10";
		case  PT_F11:
			return "PT_F11";
		case  PT_F12:
			return "PT_F12";
		case  PT_F13:
			return "PT_F13";
		case  PT_F14:
			return "PT_F14";
		case  PT_F15:
			return "PT_F15";
		case  PT_F16:
			return "PT_F16";
		case  PT_F17:
			return "PT_F17";
		case  PT_F18:
			return "PT_F18";
		case  PT_F19:
			return "PT_F19";
		case  PT_F20:
			return "PT_F20";
		case  PT_F21:
			return "PT_F21";
		case  PT_F22:
			return "PT_F22";
		case  PT_F23:
			return "PT_F23";
		case  PT_F24:
			return "PT_F24";
		case  PT_F25:
			return "PT_F25";
		case  PT_F26:
			return "PT_F26";
		case  PT_F27:
			return "PT_F27";
		case  PT_F28:
			return "PT_F28";
		case  PT_F29:
			return "PT_F29";
		case  PT_F30:
			return "PT_F30";
		case  PT_F31:
			return "PT_F31";
		case  PT_R4:
			return "PT_R4";
		case  PT_R5:
			return "PT_R5";
		case  PT_R6:
			return "PT_R6";
		case  PT_R7:
			return "PT_R7";
		case  PT_B1:
			return "PT_B1";
		case  PT_B2:
			return "PT_B2";
		case  PT_B3:
			return "PT_B3";
		case  PT_B4:
			return "PT_B4";
		case  PT_B5:
			return "PT_B5";
		case  PT_AR_EC:
			return "PT_AR_EC";
		case  PT_AR_LC:
			return "PT_AR_LC";
		case  PT_CR_IPSR:
			return "PT_CR_IPSR";
		case  PT_CR_IIP:
			return "PT_CR_IIP";
		case  PT_CFM:
			return "PT_CFM";
		case  PT_AR_UNAT:
			return "PT_AR_UNAT";
		case  PT_AR_PFS:
			return "PT_AR_PFS";
		case  PT_AR_RSC:
			return "PT_AR_RSC";
		case  PT_AR_RNAT:
			return "PT_AR_RNAT";
		case  PT_AR_BSPSTORE:
			return "PT_AR_BSPSTORE";
		case  PT_PR:
			return "PT_PR";
		case  PT_B6:
			return "PT_B6";
		case  PT_AR_BSP:
			return "PT_AR_BSP";
		case  PT_R1:
			return "PT_R1";
		case  PT_R2:
			return "PT_R2";
		case  PT_R3:
			return "PT_R3";
		case  PT_R12:
			return "PT_R12";
		case  PT_R13:
			return "PT_R13";
		case  PT_R14:
			return "PT_R14";
		case  PT_R15:
			return "PT_R15";
		case  PT_R8:
			return "PT_R8";
		case  PT_R9:
			return "PT_R9";
		case  PT_R10:
			return "PT_R10";
		case  PT_R11:
			return "PT_R11";
		case  PT_R16:
			return "PT_R16";
		case  PT_R17:
			return "PT_R17";
		case  PT_R18:
			return "PT_R18";
		case  PT_R19:
			return "PT_R19";
		case  PT_R20:
			return "PT_R20";
		case  PT_R21:
			return "PT_R21";
		case  PT_R22:
			return "PT_R22";
		case  PT_R23:
			return "PT_R23";
		case  PT_R24:
			return "PT_R24";
		case  PT_R25:
			return "PT_R25";
		case  PT_R26:
			return "PT_R26";
		case  PT_R27:
			return "PT_R27";
		case  PT_R28:
			return "PT_R28";
		case  PT_R29:
			return "PT_R29";
		case  PT_R30:
			return "PT_R30";
		case  PT_R31:
			return "PT_R31";
		case  PT_AR_CCV:
			return "PT_AR_CCV";
		case  PT_AR_FPSR:
			return "PT_AR_FPSR";
		case  PT_B0:
			return "PT_B0";
		case  PT_B7:
			return "PT_B7";
		case  PT_F6:
			return "PT_F6";
		case  PT_F7:
			return "PT_F7";
		case  PT_F8:
			return "PT_F8";
		case  PT_F9:
			return "PT_F9";
		case  PT_AR_CSD:
			return "PT_AR_CSD";
		case  PT_AR_SSD:
			return "PT_AR_SSD";
		case  PT_DBR:
			return "PT_DBR";
		case  PT_IBR:
			return "PT_IBR";
		case  PT_PMD:
			return "PT_PMD";
		default:
			return "";
	}
}

TYPE fault2type(string strTmp)
{
	TYPE faultType;
	if( strTmp == "nop" )
	{
		faultType = nop;
	}
	else if( strTmp == "timeout" )
	{
		faultType = timeout;
	}
	else if( strTmp == "1_bit_flip" )
	{
		faultType = _1_bit_flip;
	}
	else if( strTmp == "2_bit_flip" )
	{
		faultType = _2_bit_flip;
	}
	else if( strTmp == "1_bit_0" )
	{
		faultType = _1_bit_0;
	}
	else if( strTmp == "2_bit_0" )
	{
		faultType = _2_bit_0;
	}
	else if( strTmp == "1_bit_1" )
	{
		faultType = _1_bit_1;
	}
	else if( strTmp == "2_bit_1" )
	{
		faultType = _2_bit_1;
	}
	else if( strTmp == "8_low_0" )
	{
		faultType = _8_low_0;
	}
	else if( strTmp == "8_low_1" )
	{
		faultType = _8_low_1;
	}
	else if( strTmp == "8_low_error" )
	{
		faultType = _8_low_error;
	}
	else if( strTmp == "plus_1" )
	{
		faultType = _plus_1;
	}
	else if( strTmp == "plus_2" )
	{
		faultType = _plus_2;
	}
	else if( strTmp == "plus_3" )
	{
		faultType = _plus_3;
	}
	else if( strTmp == "plus_4" )
	{
		faultType = _plus_4;
	}
	else if( strTmp == "plus_5" )
	{
		faultType = _plus_5;
	}
	else
	{
		faultType = err_ftype;
	}
	return faultType;
}

string type2fault(TYPE faultType)
{
	string strTmp;
	if( faultType == nop )
	{
		strTmp = "nop";
	}
	else if( faultType == timeout )
	{
		strTmp = "timeout";
	}
	else if( faultType == _1_bit_flip )
	{
		strTmp = "1_bit_flip";
	}
	else if( faultType == _2_bit_flip )
	{
		strTmp = "2_bit_flip";
	}
	else if( faultType == _1_bit_0 )
	{
		strTmp = "1_bit_0";
	}
	else if( faultType == _2_bit_0 )
	{
		strTmp = "2_bit_0";
	}
	else if( faultType == _1_bit_1 )
	{
		strTmp = "1_bit_1";
	}
	else if( faultType == _2_bit_1 )
	{
		strTmp = "2_bit_1";
	}
	else if( faultType == _8_low_0 )
	{
		strTmp = "8_low_0";
	}
	else if( faultType == _8_low_1 )
	{
		strTmp = "8_low_1";
	}
	else if( faultType == _8_low_error )
	{
		strTmp = "8_low_error";
	}
	else if( faultType == _plus_1 )
	{
		strTmp = "plus_1";
	}
	else if( faultType == _plus_2 )
	{
		strTmp = "plus_2";
	}
	else if( faultType == _plus_3 )
	{
		strTmp = "plus_3";
	}
	else if( faultType == _plus_4 )
	{
		strTmp = "plus_4";
	}
	else if( faultType == _plus_5 )
	{
		strTmp = "plus_5";
	}
	else
	{
		strTmp = "err_ftype";
	}
	return strTmp;
}
