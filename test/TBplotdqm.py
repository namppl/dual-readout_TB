import pydrcTB

plotter1 = pydrcTB.TBplot(1000,1000,"HitMapAccuWave","hitmap")
plotter1.openFile("filename.root")
plotter1.loadTH1D("HitMapAccuWave0",27)
plotter1.Draw()
plotter1.closeFile()

plotter2 = pydrcTB.TBplot(1000,500,"disMapAccuWave","distribution")
plotter2.openFile("filename.root")
plotter2.loadTH1D("disMapAccuWave1",26)
plotter2.Draw()
plotter2.closeFile()

plotter3 = pydrcTB.TBplot(1000,1000,"wavMapWave","waveform")
plotter3.openFile("filename.root")
plotter3.loadTH1D("wavMapAccuWave2",26)
plotter3.Draw()
plotter3.closeFile()

plotter4 = pydrcTB.TBplot(1000,1000,"disMapAccuFast","distribution")
plotter4.openFile("filename.root")
plotter4.loadTH1D("HitMapAccuFast0",27)
plotter4.Draw()
plotter4.closeFile()

plotter5 = pydrcTB.TBplot(1000,1000,"HitMapAccuFast","hitmap")
plotter5.openFile("filename.root")
plotter5.loadTH1D("disMapAccuFast1",26)
plotter5.Draw()
plotter5.closeFile()
