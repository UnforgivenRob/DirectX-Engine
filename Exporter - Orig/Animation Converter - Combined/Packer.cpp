#include <string>
#include <assert.h>
#include "Packer.h"
#include "md5.h"

void Packer::packChunk(FileHandle fh, unsigned char* chunk, ChunkType type, char* chunkName, unsigned int chunkSize)
{
	 ChunkHeader header;
	 strcpy_s(header.chunkName, chunkName);
	 MD5Output output;
	 MD5Buffer(chunk, chunkSize, output);
	 header.hashNum = output.dWord_0 ^ output.dWord_1 ^ output.dWord_2 ^ output.dWord_3;
	 header.chunkSize = chunkSize;
	 header.type = type;

	 FileError res;
	 res = File::write(fh, &header, sizeof(ChunkHeader));
	 assert(res == FILE_SUCCESS);

	 res = File::write(fh, chunk, chunkSize);
	 assert(res == FILE_SUCCESS);
}
