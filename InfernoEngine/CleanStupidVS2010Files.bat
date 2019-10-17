del *.sdf /s
del *.user /s
for /d /r . %%d in (bin,obj,Debug,Release,ipch) do @if exist "%%d" rd /s/q "%%d"
del *.manifest /s
del *.obj /s
del *.tlog /s
del *.pdb /s
del *.rc /s
del *.lastbuildstate /s
del *.log /s
del *.idb /s
del *.manifest.res /s
del *.opensdf /s