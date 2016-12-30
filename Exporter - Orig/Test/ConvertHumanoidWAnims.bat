FBXConverter humanoid.fbx humanoid -skinned -removeTrans > humanoid.txt

vodka humanoid.vbo.bin humanoid.vbo -t VBO_TYPE -n humanoidVBO
vodka humanoid.mod.bin humanoid.mod -t MODELINFO_TYPE -n humanoidModInfo
vodka humanoid.ske.bin humanoid.ske -t SKEL_TYPE -n humanoidSkel
vodka humanoid.ani.bin humanoid.ani -t ANIM_TYPE -n humanoidAnim

del *.bin

FBXConverter humanoid.fbx humanoidComp -skinned -removeTrans -reduceKeyframes .25 > humanoidComp.txt

vodka humanoidComp.ani.bin humanoidComp.ani -t ANIM_TYPE -n humanoidCAnim

del *.bin

FBXConverter clipRunCycle.fbx RunCycle -skinned -removeTrans -AnimOnly > runCycle.txt
FBXConverter clipWalkCycle.fbx WalkCycle -skinned -removeTrans -AnimOnly  > WalkCycle.txt
FBXConverter clipStopCycle.fbx StopCycle -skinned -removeTrans -AnimOnly > StopCycle.txt
FBXConverter heroRunJump.fbx Jump -skinned -removeTrans -AnimOnly > Jump.txt

vodka RunCycle.ani.bin RunCycle.ani -t ANIM_TYPE -n RunCycleAnim
vodka WalkCycle.ani.bin WalkCycle.ani -t ANIM_TYPE -n WalkCycleAnim
vodka StopCycle.ani.bin StopCycle.ani -t ANIM_TYPE -n StopCycleAnim
vodka Jump.ani.bin Jump.ani -t ANIM_TYPE -n JumpAnim

del *.bin

liu humanoid.rob -n humanoid -v 1.0

del *.vbo
del *.mod
del *.ani
del *.ske
del *.tex