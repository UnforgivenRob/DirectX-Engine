::FBXConverter sphere.fbx common -t default.tga > common.txt

::vodka common.mod.bin common.mod -t MODELINFO_TYPE -n CommonInfo
::vodka common.vbo.bin common.vbo -t VBO_TYPE -n Common_VBO
::vodka common0.tex.bin common.tex -t TEXTURE_TYPE -n Common_Texture

::del *.bin

::liu common.rob -n Common -v 1.0

::del *.mod
::del *.vbo
::del *.tex


FBXConverter StaticMeshes/space_frigate.fbx space_frigate -NotSkinned -NoRemoveTrans > space_frigate.txt

vodka space_frigate.mod.bin space_frigate.mod -t MODELINFO_TYPE -n space_frigateInfo
vodka space_frigate.vbo.bin space_frigate.vbo -t VBO_TYPE -n space_frigate_VBO
vodka space_frigate0.tex.bin space_frigate.tex -t TEXTURE_TYPE -n space_frigate_Text

del *.bin

liu space_frigate.rob -n Space_Frigate -v 1.0

del *.mod
del *.vbo
del *.tex


::FBXConverter box.fbx box -t Diffuse.tga > box.txt

::vodka box.mod.bin box.mod -t MODELINFO_TYPE -n boxInfo
::vodka box.vbo.bin box.vbo -t VBO_TYPE -n box_VBO
::vodka box0.tex.bin box.tex -t TEXTURE_TYPE -n box_Texture

::del *.bin

::liu box.rob -n Box -v 1.0

::del *.mod
::del *.vbo
::del *.tex


::FBXConverter warbear_binary.fbx warbear -t warbear.tga -e 1 polarbear.tga > warbear.txt

::vodka warbear.mod.bin warbear.mod -t MODELINFO_TYPE -n warbearInfo
::vodka warbear.vbo.bin warbear.vbo -t VBO_TYPE -n warbear_VBO
::vodka warbear0.tex.bin warbear0.tex -t TEXTURE_TYPE -n warbear_Texture0
::vodka warbear1.tex.bin warbear1.tex -t TEXTURE_TYPE -n warbear_Texture1

::del *.bin

::liu warbear.rob -n Warbear -v 1.0

::del *.mod
::del *.vbo
del *.tex


FBXConverter StaticMeshes/buggy.fbx buggy > buggy.txt

vodka buggy.mod.bin buggy.mod -t MODELINFO_TYPE -n buggyInfo
vodka buggy.vbo.bin buggy.vbo -t VBO_TYPE -n buggy_VBO
vodka buggy0.tex.bin buggy0.tex -t TEXTURE_TYPE -n buggy_Texture0

del *.bin

liu buggy.rob -n Buggy -v 1.0

del *.mod
del *.vbo
del *.tex

FBXConverter StaticMeshes/astroboy_binary.fbx astroboy -t astroboy.tga > astroboy.txt


vodka astroboy.mod.bin astroboy.mod -t MODELINFO_TYPE -n astroboyInfo
vodka astroboy.vbo.bin astroboy.vbo -t VBO_TYPE -n astroboy_VBO
vodka astroboy0.tex.bin astroboy0.tex -t TEXTURE_TYPE -n astroboy_Texture0
vodka astroboy1.tex.bin astroboy1.tex -t TEXTURE_TYPE -n astroboy_Texture1

del *.bin

liu astroboy.rob -n Astroboy -v 1.0

del *.mod
del *.vbo
del *.tex