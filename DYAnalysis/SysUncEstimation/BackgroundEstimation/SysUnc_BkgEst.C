#include "SysUncTool_BkgEst.h"

void SysUnc_BkgEst(TString version="", DYana::var thevar=DYana::var::ALLvar)
{
   if (thevar != DYana::var::ALLvar) {
	SysUncTool_BkgEst *tool = new SysUncTool_BkgEst(version, thevar);
	tool->SysUncEstimation();
	
	tool->MakeCanvas_ForEachBkgMethod();
	tool->MakeCanvas_SysUnc_StatSystTotal();
	tool->Save_All();
   } else {
      for (int i=0; i<ALLvar2; i++) {
         if (i==ALLvar) continue;
         DYana::var thevar_i = static_cast<DYana::var>(i);
         SysUncTool_BkgEst *tool = new SysUncTool_BkgEst(version, thevar_i);
         tool->SysUncEstimation();

         tool->MakeCanvas_ForEachBkgMethod();
         tool->MakeCanvas_SysUnc_StatSystTotal();
         tool->Save_All();
      }
   }
}
