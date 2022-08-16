import ROOT
import pydrcTB
import argparse

parser=argparse.ArgumentParser()
parser.add_argument("--in_root",type=str,default="test.root",help="input root file name")
parser.add_argument("--save_name",type=str,default="test_skimmed.root",help="skimmed root file name")

parser.add_argument("--mod",type=str,default="fast",help="fast or wave")
parser.add_argument("--mapping",type=str,default="mapping_data_MCPPMT_positiveSignal.csv",help="mapping file")
parser.add_argument("--pedestal",type=str,default="ped_236.csv",help="pedestal file")

args=parser.parse_args()
mod=args.mod

# open root file and get Tree
in_file = ROOT.TFile(args.in_root, "READ")
out_file = ROOT.TFile(args.save_name, "RECREATE")

atree = in_file.Get("events")
atree.GetEntry(0)
anevt = getattr(atree,"TBevt")
# anevt = ROOT.TBevt("TBfastmode")()

btree = ROOT.TTree("skimmed_events", "fastmode events")
btree.Branch("TBevt", anevt)

# load mapping and pedestal
utility = pydrcTB.TButility()
utility.loading(args.mapping)
utility.loadped(args.pedestal)
channelsize = 32

print("Read entry")
for ievt in range(atree.GetEntries()):
    # load each entry
    atree.GetEntry(ievt)
    anevt = getattr(atree,"TBevt")

    # initilize value - important
    adc_ps = 0.
    adc_md = 0.

    # check data of each entry
    for imid in list_mid:
        for ich in list_channel:
            # TODO cannot convert ROOT TBcid & boost::python TBcid automatically
            cidboost = pydrcTB.TBcid( imid+1, ich+1 ) # mid 1 - 15, ch 1 - 32
            cidroot  = ROOT.TBcid( imid+1, ich+1 )

            adet = utility.find(cidboost)# mapping information

            if adet.det() == ROOT.TBdetector.detid.preshower:
                adata = anevt.data( cidroot )
                adc_ps += adata.adc()
            elif adet.det() == ROOT.TBdetector.detid.muon:
                adata = anevt.data( cidroot )
                adc_md += adata.adc()

    if adc_ps > thr_ps or adc_md > thr_md:
        continue

    btree.Fill()
print("Entry ended")
