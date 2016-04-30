//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "UnitTest.h"

#include "File.h"

// used for teardown
FileHandle fh_file1;
FileHandle fh_file3;


//---------------------------------------------------------------------------
// Test Files:
// Single step through, set break points, but do not modify it
// To Run all tests: Debug-> Start without debugging
//---------------------------------------------------------------------------


TEST( simple_open, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file1.bin", FILE_WRITE );
   // used to clean up at end of test
   fh_file1 = fh;

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

}

TEST( simple_open_close, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file2.bin", FILE_WRITE );

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}


TEST( simple_open_open, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file3.bin", FILE_WRITE );
   fh_file3 = fh;  // used for teardown

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

   // should fail
   ferror = File::open(fh, "file3.bin", FILE_WRITE );
   CHECK( ferror == FILE_OPEN_FAIL );


}

TEST( simple_open_error_name, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, 0, FILE_WRITE );

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_OPEN_FAIL );



}

TEST( simple_open_close_close, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file4.bin", FILE_WRITE );

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_CLOSE_FAIL );
}


TEST( simple_close_fail, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   fh = (FileHandle)0x55555;

   ferror = File::close( fh );
   CHECK( ferror == FILE_CLOSE_FAIL );
}



TEST( simple_write, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file5.bin", FILE_WRITE );

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}


TEST( simple_write_fail_buffer, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   ferror = File::open(fh, "file6.bin", FILE_WRITE );

   // make sure it opened, also check the directory
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, 0, 8 );
   CHECK( ferror == FILE_WRITE_FAIL );

   ferror = File::write( fh, data, 0 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::write( fh, data, (size_t)-1 );
   CHECK( ferror == FILE_WRITE_FAIL);

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}

TEST( simple_write_fail, FileBasic_group)
{
   FileHandle fh;
   FileHandle fh2;
   FileError  ferror;

   ferror = File::open(fh, "file7.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::open(fh2, "file8.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh2 );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh2, data, 8 );
   CHECK( ferror == FILE_WRITE_FAIL );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}


TEST( simple_write_read, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file9.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------

   FileHandle fh2;
   ferror = File::open(fh2, "file9.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );
   
   unsigned char dataFromFile[8];
   ferror = File::read( fh2, dataFromFile, 8 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( memcmp( data, dataFromFile, 8 ) == 0 );

   ferror = File::close( fh2 );
   CHECK( ferror == FILE_SUCCESS );
}

TEST( simple_write_read_Buffer_Error, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file10.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------

   FileHandle fh2;
   ferror = File::open(fh2, "file10.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );
   
   unsigned char dataFromFile[8];
   ferror = File::read( fh2, 0, 8 );
   CHECK( ferror == FILE_READ_FAIL );

   ferror = File::read( fh2, dataFromFile, (size_t)-1 );
   CHECK( ferror == FILE_READ_FAIL );

   ferror = File::close( fh2 );
   CHECK( ferror == FILE_SUCCESS );
}

TEST( simple_write_read_fail, FileBasic_group)
{
   FileHandle A;
   FileHandle fh2;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(A, "file11.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( A, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   // Microsoft sucks, these next two lines need to be there, but it cant due to aliasing
   //  ferror = File::close( A );
   //  CHECK( ferror == FILE_SUCCESS );
 
   ferror = File::open(fh2, "file12.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

    ferror = File::close( A );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::write( fh2, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh2 );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------

   ferror = File::open(fh2, "file12.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );
   
   unsigned char dataFromFile[8];
   ferror = File::read( A, dataFromFile, 8 );
   CHECK( ferror == FILE_READ_FAIL );

   ferror = File::close( fh2 );
   CHECK( ferror == FILE_SUCCESS );
}

TEST( simple_write_read_SEEK, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file13.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- Open to Read  --------------------------------

   ferror = File::open(fh, "file13.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );

   //----------- Seek ------------------------------------------

   ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char returnData;
   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0xaa );

   ferror = File::seek( fh, FILE_SEEK_END, -1 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0x22 );

   ferror = File::seek( fh, FILE_SEEK_CURRENT, -4 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0xee );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}


TEST( simple_write_read_SEEK_error, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file14.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- Open to Read  --------------------------------

   ferror = File::open(fh, "file14.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );

   //----------- Seek ------------------------------------------

   ferror = File::seek( fh, FILE_SEEK_BEGIN, -1 );
   CHECK( ferror == FILE_SEEK_FAIL );

   ferror = File::seek( fh, FILE_SEEK_CURRENT, -55 );
   CHECK( ferror == FILE_SEEK_FAIL );

   ferror = File::seek( fh, FILE_SEEK_END, -77 );
   CHECK( ferror == FILE_SEEK_FAIL );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}

TEST( simple_write_read_TELL, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file15.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- OPEN to READ -----------------------------------

   int offset;

   ferror = File::open(fh, "file15.bin", FILE_READ );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 0 );
   
   ferror = File::seek( fh, FILE_SEEK_END, 0 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 8 );

   //----------- Seek ------------------------------------------

   ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 0 );


   unsigned char returnData;
   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0xaa );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 1 );


   ferror = File::seek( fh, FILE_SEEK_END, -1 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 7 );

   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0x22 );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 8 );

   ferror = File::seek( fh, FILE_SEEK_CURRENT, -4 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 4 );

   ferror = File::read( fh, &returnData, 1 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( returnData == 0xee );

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 5 );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}


TEST( simple_write_read_TELL_error, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file16.bin", FILE_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- OPEN to READ -----------------------------------

   int offset;

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_TELL_FAIL );
  
}

TEST( simple_writeread, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file17.bin", FILE_READ_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------
   ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
   CHECK( ferror == FILE_SUCCESS );

   int offset;

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 0 );

   unsigned char dataFromFile[8];

   ferror = File::read( fh, dataFromFile, 8 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( memcmp( data, dataFromFile, 8 ) == 0 );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}



TEST( simple_writeread_flush, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file18.bin", FILE_READ_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------
   ferror = File::flush( fh );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
   CHECK( ferror == FILE_SUCCESS );

   int offset;

   ferror = File::tell( fh, offset );
   CHECK( ferror == FILE_SUCCESS );
   CHECK( offset == 0 );

   unsigned char dataFromFile[8];

   ferror = File::read( fh, dataFromFile, 8 );
   CHECK( ferror == FILE_SUCCESS );

   CHECK( memcmp( data, dataFromFile, 8 ) == 0 );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );
}




TEST( simple_writeread_flush_error, FileBasic_group)
{
   FileHandle fh;
   FileError  ferror;

   //----------- WRITE ------------------------------------------

   ferror = File::open(fh, "file19.bin", FILE_READ_WRITE );
   CHECK( ferror == FILE_SUCCESS );

   unsigned char data[8] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x11,0x22};

   ferror = File::write( fh, data, 8 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh );
   CHECK( ferror == FILE_SUCCESS );

   //----------- READ ------------------------------------------
   ferror = File::flush( fh );
   CHECK( ferror == FILE_FLUSH_FAIL );

  
}





TEST( Shutdown, FileBasic_group)
{
   FileError  ferror;

   ferror = File::close( fh_file1 );
   CHECK( ferror == FILE_SUCCESS );

   ferror = File::close( fh_file3 );
   CHECK( ferror == FILE_SUCCESS );

}