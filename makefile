all:
	make -C Builds/LinuxMakefile
	./Builds/LinuxMakefile/build/Mental

Projucer:
	./libs/JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer

push:
	git add . && git commit -m "$(m)" && git push

clean:
	make -C Builds/LinuxMakefile clean
