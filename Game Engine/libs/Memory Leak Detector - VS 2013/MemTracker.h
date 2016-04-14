#ifndef MEM_TRACKER_H
#define MEM_TRACKER_H

//
// MemTracker Instructions:
//
//    This library finds memory leaks for new and new[]
//    It displays the location,size of each memory leak
//
//    0) Import the library and header
//       Debug:
//           copy MemTrackerDebug.lib, MemTracker.h to project directory
//           add files to project
//       Release:
//           copy MemTrackerRelease.lib, MemTracker.h to project directory
//           add files to project
//       Note: you can't have both libraries installed for Debug or Release
//           exclude one library file -> Exclude From Build
//
//    1) Search through your code
//       replace every "new" call with "newTracker"
//       #include "MemTracker.h" to each cpp file that has a new call
//
//    2) Add to Main.cpp
//       #include "MemTracker.h" add include
//       refactor and add macros
//
//       int main()
//       {
//           MEM_TRACKER_START
//
//           ... your code ...
//
//           MEM_TRACKER_END
//       }
//
//    3) Make sure your project is in Static Lib mode
//       Debug:
//          Project->Properties...->C/C++->Runtime Libray
//             set to:  Multi-threaded Debug (/MTd)
//       Release:
//          Project->Properties...->C/C++->Runtime Libray
//             set to:  Multi-threaded (/MT)


// Uncomment this line to disable memory tracker
// #define MEM_TRACKER_DISABLED


#ifdef MEM_TRACKER_DISABLED
	#define newTracker	new
#else
	#define newTracker  new( __FILE__, __LINE__)
#endif

// Mem Tracker - finds memory leaks
#define MEM_TRACKER_START  MemTracker::Create();{
#define MEM_TRACKER_END	   }MemTracker::PrintLeaks();

#define DEFAULT_HEAP_SIZE (10 * 1024 * 1024)

class Mem;  // forward declare
class Heap;

class MemTracker
{
public:
	static void Create( int heapSize = DEFAULT_HEAP_SIZE );
	static void PrintLeaks();
	static void Destroy();
	static Heap *getHeap();

private:
	// big four
	MemTracker();
	~MemTracker();
	MemTracker(const MemTracker &);
	MemTracker & operator=(const MemTracker &);

	// methods
	static MemTracker *privGetInstance();

	// data
	Mem *pMem;
	Heap *pHeap;
};

// Overloaded new/delete prototypes:
	void * operator new(size_t inSize, char *inName, int lineNum);
	void operator delete(void *p, char *inName, int lineNum);

	void * operator new(size_t inSize);
	void operator delete(void *p);

	void * operator new[](size_t inSize, char *inName, int lineNum);
	void operator delete[](void *p, char *inName, int lineNum);

	void * operator new[](size_t inSize);
	void operator delete[](void *p);


#endif