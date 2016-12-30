#include "FBX_Loader.h"
#include "PackageHeaders.h"
#include "ModelManager.h"
#include "FileSystem.h"
//#include "TextureManager.h"
//#include "AnimationManager.h"
//#include "CameraManager.h"
//#include "Animatable_GameObject.h"
//#include "GraphicsObject_TextSkinned.h"
//
//loads simple 1 texture, 1 model fbx models
void FBX_Loader::LoadSimple(Model_ID modelID, /*Texture_ID textID,*/ const char* filename)
{
	FileHandle fh;
	FileError err;

	err = File::open(fh, filename, FILE_READ);
	assert(err == FILE_SUCCESS);

	PackageHeader pheader;

	err = File::read(fh, &pheader, sizeof(PackageHeader));
	assert(err == FILE_SUCCESS);

	ChunkHeader chunkHead;
	//Read Model_Info Chunk
	err = File::read(fh, &chunkHead, sizeof(chunkHead));
	assert(err == FILE_SUCCESS);
	assert(chunkHead.type == ChunkType::MODELINFO_TYPE);

	Model_Info mInfo;
	err = File::read(fh, &mInfo, sizeof(Model_Info));
	assert(err == FILE_SUCCESS);

	//read VBO chunk
	err = File::read(fh, &chunkHead, sizeof(chunkHead));
	assert(err == FILE_SUCCESS);
	assert(chunkHead.type == ChunkType::VBO_TYPE);

	VBO_header vbohead;
	err = File::read(fh, &vbohead, sizeof(VBO_header));
	assert(err == FILE_SUCCESS);

	float* vertex_Buffer = new float[vbohead.numVerts * 8 * sizeof(float)];
	int* index_Buffer = new int[vbohead.numIndices * sizeof(int)];

	err = File::read(fh, vertex_Buffer, vbohead.numVerts * 8 * sizeof(float));
	assert(err == FILE_SUCCESS);

	err = File::read(fh, index_Buffer,vbohead.numIndices * sizeof(int));
	assert(err == FILE_SUCCESS);

	ModelManager::create(modelID, filename/* (Vert_Final_Skinned*)vertex_Buffer, index_Buffer, vbohead, mInfo.center, mInfo.radius*/);
	
	delete[] vertex_Buffer;
	delete[] index_Buffer;

	////load texture
	//for(int i = 0; i < mInfo.numTexts; i++)
	//{
	//	err = File::read(fh, &chunkHead, sizeof(ChunkHeader));
	//	assert(err == FILE_SUCCESS);
	//	assert(chunkHead.type == ChunkType::TEXTURE_TYPE);

	//	Text_Header tHead;
	//	err = File::read(fh, &tHead, sizeof(Text_Header));
	//	assert(err == FILE_SUCCESS);

	//	GLbyte* tBuff = new GLbyte[chunkHead.chunkSize - sizeof(Text_Header)];
	//
	//	err = File::read(fh, tBuff, chunkHead.chunkSize - sizeof(Text_Header));
	//	assert(err == FILE_SUCCESS);

	//	TextureManager::create(textID, tBuff, tHead.width, tHead.height, tHead.iComponents, tHead.eFormat,  GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

	//	delete[] tBuff;

	//	
	//}

	err = File::close(fh);
	assert(err == FILE_SUCCESS);
}
//
////loads complex models with multiple textures and/or models
//void FBX_Loader::LoadComplex(Model_ID* modelIDs, int numModels, Texture_ID* textIDs, int numTexts, const char* filename)
//{
//	FileHandle fh;
//	FileError err;
//
//	err = File::open(fh, filename, FILE_READ);
//	assert(err == FILE_SUCCESS);
//
//	PackageHeader pheader;
//
//	err = File::read(fh, &pheader, sizeof(PackageHeader));
//	assert(err == FILE_SUCCESS);
//
//	ChunkHeader chunkHead;
//	//Read Model_Info Chunk
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::MODELINFO_TYPE);
//
//	Model_Info mInfo;
//	err = File::read(fh, &mInfo, sizeof(Model_Info));
//	assert(err == FILE_SUCCESS);
//
//	for(int i = 0; i < numModels; i++)
//	{
//		//read VBO chunk
//		err = File::read(fh, &chunkHead, sizeof(chunkHead));
//		assert(err == FILE_SUCCESS);
//		assert(chunkHead.type == ChunkType::VBO_TYPE);
//
//		VBO_header vbohead;
//		err = File::read(fh, &vbohead, sizeof(VBO_header));
//		assert(err == FILE_SUCCESS);
//
//		float* vertex_Buffer = new float[vbohead.numVerts * 8 * sizeof(float)];
//		int* index_Buffer = new int[vbohead.numIndices * sizeof(int)];
//
//		err = File::read(fh, vertex_Buffer, vbohead.numVerts * 8 * sizeof(float));
//		assert(err == FILE_SUCCESS);
//
//		err = File::read(fh, index_Buffer,vbohead.numIndices * sizeof(int));
//		assert(err == FILE_SUCCESS);
//
//		ModelManager::create(modelIDs[i], (Vert_Final_Skinned*)vertex_Buffer, index_Buffer, vbohead, mInfo.center, mInfo.radius);
//	
//		delete[] vertex_Buffer;
//		delete[] index_Buffer;
//	}
//
//	//load texture
//	for(int i = 0; i < numTexts; i++)
//	{
//		err = File::read(fh, &chunkHead, sizeof(ChunkHeader));
//		assert(err == FILE_SUCCESS);
//		assert(chunkHead.type == ChunkType::TEXTURE_TYPE);
//
//		Text_Header tHead;
//		err = File::read(fh, &tHead, sizeof(Text_Header));
//		assert(err == FILE_SUCCESS);
//
//		GLbyte* tBuff = new GLbyte[chunkHead.chunkSize - sizeof(Text_Header)];
//	
//		err = File::read(fh, tBuff, chunkHead.chunkSize - sizeof(Text_Header));
//		assert(err == FILE_SUCCESS);
//
//		TextureManager::create(textIDs[i], tBuff, tHead.width, tHead.height, tHead.iComponents, tHead.eFormat,  GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
//
//		delete[] tBuff;
//	}
//
//	err = File::close(fh);
//	assert(err == FILE_SUCCESS);
//}
//
//void FBX_Loader::LoadSkinnedSimple(Model_ID modelID, Anim_ID animID, const char* filename, Skeleton* outSkel, Animatable_GameObject* go)
//{
//	animID;
//	FileHandle fh;
//	FileError err;
//
//	err = File::open(fh, filename, FILE_READ);
//	assert(err == FILE_SUCCESS);
//
//	PackageHeader pheader;
//
//	err = File::read(fh, &pheader, sizeof(PackageHeader));
//	assert(err == FILE_SUCCESS);
//
//	ChunkHeader chunkHead;
//	//Read Model_Info Chunk
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::MODELINFO_TYPE);
//
//	Model_Info mInfo;
//	err = File::read(fh, &mInfo, sizeof(Model_Info));
//	assert(err == FILE_SUCCESS);
//
//	//read VBO chunk
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::VBO_TYPE);
//
//	VBO_header vbohead;
//	err = File::read(fh, &vbohead, sizeof(VBO_header));
//	assert(err == FILE_SUCCESS);
//	assert( vbohead.skinnned );
//
//	Vert_Final_Skinned* vertex_Buffer = new Vert_Final_Skinned[ vbohead.numVerts ];
//	int* index_Buffer = new int[vbohead.numIndices * sizeof(int)];
//
//	err = File::read( fh, vertex_Buffer, vbohead.numVerts * sizeof( Vert_Final_Skinned ) );
//	assert(err == FILE_SUCCESS);
//
//	err = File::read(fh, index_Buffer,vbohead.numIndices * sizeof(int));
//	assert(err == FILE_SUCCESS);
//
//	ModelManager::create(modelID, (Vert_Final_Skinned*)vertex_Buffer, index_Buffer, vbohead, mInfo.center, mInfo.radius);
//	
//	delete[] vertex_Buffer;
//	delete[] index_Buffer;
//
//	////load texture
//	//for(int i = 0; i < mInfo.numTexts; i++)
//	//{
//	//	err = File::read(fh, &chunkHead, sizeof(ChunkHeader));
//	//	assert(err == FILE_SUCCESS);
//	//	assert(chunkHead.type == ChunkType::TEXTURE_TYPE);
//
//	//	Text_Header tHead;
//	//	err = File::read(fh, &tHead, sizeof(Text_Header));
//	//	assert(err == FILE_SUCCESS);
//
//	//	GLbyte* tBuff = new GLbyte[chunkHead.chunkSize - sizeof(Text_Header)];
//	//
//	//	err = File::read(fh, tBuff, chunkHead.chunkSize - sizeof(Text_Header));
//	//	assert(err == FILE_SUCCESS);
//
//	//	TextureManager::create(textID, tBuff, tHead.width, tHead.height, tHead.iComponents, tHead.eFormat,  GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
//
//	//	delete[] tBuff;
//	//}
//
//	AnimationHeader inAH;
//
//	err = File::read(fh, &inAH, sizeof(AnimationHeader));
//	assert(err == FILE_SUCCESS);
//
//	Hierarchy_Data* inData = new Hierarchy_Data[inAH.numBones];
//
//	err = File::read(fh, inData, (inAH.numBones) * sizeof(Hierarchy_Data));
//	assert(err == FILE_SUCCESS);
//
//	outSkel->setSkeletonData(go, inAH.numBones, inData);
//
//	//set up all the animations
//	Anim_Header inAnim;
//	for(int i = 0; i < 1; i++)
//	{
//		err = File::read(fh, &inAnim, sizeof(Anim_Header));
//		assert(err == FILE_SUCCESS);
//
//		int* times = new int[inAnim.numKeyFrames];
//
//		err = File::read(fh, times, inAnim.numKeyFrames * sizeof(int));
//		assert(err == FILE_SUCCESS);
//
//		BoneTransform_Reduced* inboneData = new BoneTransform_Reduced[inAH.numBones * inAnim.numKeyFrames];
//		
//		err = File::read(fh, inboneData, inAH.numBones * inAnim.numKeyFrames * sizeof(BoneTransform_Reduced));
//		assert(err == FILE_SUCCESS); 
//
//		Time interval;
//		if(inAH.fps == 30)
//		{
//			interval = Time(TIME_NTSC_30_FRAME);
//		}
//		else
//		{
//			interval = Time(TIME_NTSC_FRAME);
//		}
//
//		AnimationManager::create( animID, inAH.numBones, inAnim.numKeyFrames, inAnim.maxTime, times, interval, inboneData, inData, 0, false );
//		
//		delete inboneData;
//	}
//
//	err = File::close(fh);
//	assert(err == FILE_SUCCESS);
//	go->setAnimation( false );
//	Matrix id(IDENTITY);
//	outSkel->update(Time(TIME_ZERO), id, CameraManager::getActive()->getViewMat());
//	outSkel->setBonePose();
//
//
//}
//
//void FBX_Loader::LoadSkinnedComplex( Model_ID* modelIDs, const int numModels, Texture_ID* textureIDs, const int numTextures,
//									Anim_ID* animIDs, const int numAnims, const char* filename, Skeleton* outSkel, Animatable_GameObject* go, 
//									GraphicsObject_Skinned* graphObj, bool bOnGPU )
//{
//	numModels;
//	FileHandle fh;
//	FileError err;
//	int numChunksProcessed = 0;
//
//	err = File::open(fh, filename, FILE_READ);
//	assert(err == FILE_SUCCESS);
//
//	PackageHeader pheader;
//
//	err = File::read(fh, &pheader, sizeof(PackageHeader));
//	assert(err == FILE_SUCCESS);
//
//	//Read Model_Info Chunk
//	ChunkHeader chunkHead;
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::MODELINFO_TYPE);
//
//	Model_Info mInfo;
//	err = File::read(fh, &mInfo, sizeof(Model_Info));
//	assert(err == FILE_SUCCESS);
//
//	numChunksProcessed++;
//
//	//read VBO chunk
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::VBO_TYPE);
//
//	VBO_header vbohead;
//	err = File::read(fh, &vbohead, sizeof(VBO_header));
//	assert(err == FILE_SUCCESS);
//	assert( vbohead.skinnned );
//
//	Vert_Final_Skinned* vertex_Buffer = new Vert_Final_Skinned[ vbohead.numVerts ];
//	int* index_Buffer = new int[vbohead.numIndices * sizeof(int)];
//
//	err = File::read( fh, vertex_Buffer, vbohead.numVerts * sizeof( Vert_Final_Skinned ) );
//	assert(err == FILE_SUCCESS);
//
//	err = File::read(fh, index_Buffer,vbohead.numIndices * sizeof(int));
//	assert(err == FILE_SUCCESS);
//
//	ModelManager::createSkinned(modelIDs[0], vertex_Buffer, index_Buffer, vbohead, mInfo.center, mInfo.radius);
//	
//	delete[] vertex_Buffer;
//	delete[] index_Buffer;
//
//	numChunksProcessed++;
//
//	//load texture
//	for(int i = 0; i < numTextures; i++)
//	{
//		err = File::read(fh, &chunkHead, sizeof(ChunkHeader));
//		assert(err == FILE_SUCCESS);
//		assert(chunkHead.type == ChunkType::TEXTURE_TYPE);
//
//		Text_Header tHead;
//		err = File::read(fh, &tHead, sizeof(Text_Header));
//		assert(err == FILE_SUCCESS);
//
//		GLbyte* tBuff = new GLbyte[chunkHead.chunkSize - sizeof(Text_Header)];
//	
//		err = File::read(fh, tBuff, chunkHead.chunkSize - sizeof(Text_Header));
//		assert(err == FILE_SUCCESS);
//
//		TextureManager::create(textureIDs[i], tBuff, tHead.width, tHead.height, tHead.iComponents, tHead.eFormat,  GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
//		((GraphicsObject_TextSkinned*)go->getGraphicsObject())->setTexture( textureIDs[i] );
//		delete[] tBuff;
//
//		numChunksProcessed++;
//	}
//
//	//read Skel chunk
//	err = File::read(fh, &chunkHead, sizeof(chunkHead));
//	assert(err == FILE_SUCCESS);
//	assert(chunkHead.type == ChunkType::SKEL_TYPE);
//	
//	Skel_Header inSH;
//	err = File::read(fh, &inSH, sizeof(Skel_Header));
//	assert(err == FILE_SUCCESS);
//	
//	Hierarchy_Data* inData = new Hierarchy_Data[inSH.numBones];
//	err = File::read(fh, inData, (inSH.numBones) * sizeof(Hierarchy_Data));
//	assert(err == FILE_SUCCESS);
//
//	Matrix* invBindPose1 = new Matrix[ inSH.numBones ];
//	err = File::read(fh, invBindPose1, ( inSH.numBones ) * sizeof(Matrix));
//	assert(err == FILE_SUCCESS);
//
//	outSkel->setSkeletonData(go, inSH.numBones, inData);
//
//	numChunksProcessed++;
//
//	int tmp = 0;
//	AnimationHeader inAH = AnimationHeader();
//	while( numChunksProcessed < pheader.numChunks )
//	{
//		//read Anim chunk
//		err = File::read(fh, &chunkHead, sizeof(chunkHead));
//		assert(err == FILE_SUCCESS);
//		assert(chunkHead.type == ChunkType::ANIM_TYPE);
//
//		err = File::read(fh, &inAH, sizeof(AnimationHeader));
//		assert(err == FILE_SUCCESS);
//
//		//set up all the animations
//		Anim_Header inAnim;
//		for(int i = 0; i < inAH.numAnims; i++)
//		{
//			err = File::read(fh, &inAnim, sizeof(Anim_Header));
//			assert(err == FILE_SUCCESS);
//
//			int* times = new int[inAnim.numKeyFrames];
//
//			err = File::read(fh, times, inAnim.numKeyFrames * sizeof(int));
//			assert(err == FILE_SUCCESS);
//
//			BoneTransform_Reduced* inboneData = new BoneTransform_Reduced[inAH.numBones * inAnim.numKeyFrames];
//		
//			err = File::read(fh, inboneData, inAH.numBones * inAnim.numKeyFrames * sizeof(BoneTransform_Reduced));
//			assert(err == FILE_SUCCESS); 
//
//			Time interval;
//			if(inAH.fps == 30)
//			{
//				interval = Time(TIME_NTSC_30_FRAME);
//			}
//			else
//			{
//				interval = Time(TIME_NTSC_FRAME);
//			}
//
//			AnimationManager::create( animIDs[tmp + i], inAH.numBones, inAnim.numKeyFrames, inAnim.maxTime, times, interval, inboneData, inData, invBindPose1, bOnGPU );
//			
//			delete[] times;
//			delete[] inboneData;
//		}
//		numChunksProcessed++;
//		tmp+= inAH.numAnims;
//		assert(tmp <= numAnims);
//	}
//
//	err = File::close(fh);
//	assert(err == FILE_SUCCESS);
//
//	go->setAnimation( bOnGPU);
//	graphObj->setModel(modelIDs[0]);
//	graphObj->setUpBoneMats(inSH.numBones);
//	go->setBindPose(invBindPose1, inSH.numBones);
//	Matrix id(IDENTITY);
//	//outSkel->update(Time(TIME_ZERO), id, CameraManager::getActive()->getViewMat());
//	outSkel->setBonePose();
//
//}
//
