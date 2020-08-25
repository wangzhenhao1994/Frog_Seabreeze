/// \file
/// \ingroup Tutorials
/// Example of `rootlogon.C`.
/// The macro `rootlogon.C` in the current working directory, is executed when
/// `root` starts unless the option `-n` is used.
///
/// \macro_code
///
/// \author Rene Brun
void rootlogon()
{
        gROOT->ProcessLine(".include .");
        gROOT->ProcessLine(".include ./SeaBreeze/include");
        gROOT->ProcessLine(".include ./PI_E709-2.0.0-INSTALL/PI_E709/libpi_pi_gcs2_x86_64/include");
        gSystem->Load("/mnt/c/Users/wangz/Desktop/Frog_Seabreeze/SeaBreeze/lib/libseabreeze.so");
        gSystem->Load("./PI_E709-2.0.0-INSTALL/PI_E709/libpi_pi_gcs2_x86_64/lib/libpi_pi_gcs2.so.2.16.0");
}
{
   printf("\nWelcome to the ROOT tutorials\n\n");
   printf("\nType \".x demos.C\" to get a toolbar from which to execute the demos\n");
   printf("\nType \".x demoshelp.C\" to see the help window\n\n");
   printf("==> Many tutorials use the file hsimple.root produced by hsimple.C\n");
   printf("==> It is recommended to execute hsimple.C before any other script\n\n");
}
