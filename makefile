all:
	make -C Builds/LinuxMakefile
	./Builds/LinuxMakefile/build/Mental

projucer:
	./libs/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer

push:
	git add . && git commit -m "$(m)" && git push