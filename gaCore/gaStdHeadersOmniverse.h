#pragma once

/*****************************************************************************/
/**
* 
*/
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <filesystem>
  #include <conio.h>
#else
  #include <experimental/filesystem>
#endif

#include <mutex>
#include <condition_variable>