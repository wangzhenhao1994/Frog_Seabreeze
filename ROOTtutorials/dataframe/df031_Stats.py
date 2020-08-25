## \file
## \ingroup tutorial_dataframe
## \notebook
##
## Extract the statistics relative to RDataFrame columns and store them
##  in TStatistic instances.
##
## \macro_code
## \macro_output
##
## \date April 2019
## \author Danilo Piparo

import ROOT

# Create a data frame and add two columns: one for the values and one for the weight.
r = ROOT.RDataFrame(256);
rr = r.Define("v", "rdfentry_")\
      .Define("w", "return 1./(v+1)")

# Now extract the statistics, weighted, unweighted
stats_iu = rr.Stats("v")
stats_iw = rr.Stats("v", "w")

# Now print them: they are all identical of course!
stats_iu.Print()
stats_iw.Print()
