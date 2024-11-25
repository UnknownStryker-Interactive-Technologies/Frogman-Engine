#pragma once


// Frogman Engine uses /Wx and /W4. These lines of warning disablers are added by the developer of the engine.
#pragma warning(push)
#pragma warning(disable: 4244) 
/* Silience!
1 > C:\Users\leeho\OneDrive\문서\GitHub\Frogman - Engine\SDK\Third - Party\Libraries\taskflow - 3.8.0\taskflow\core\atomic_notifier.hpp(187, 24) : warning C4244 : '=' : conversion from 'uint64_t' to 'uint32_t', possible loss of data
*/
#pragma warning(disable: 4324) 
/* Silience!
1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Third-Party\Libraries\taskflow-3.8.0\taskflow\core\tsq.hpp(298,1): warning C4324: 'tf::BoundedTaskQueue<tf::Node *,8>': structure was padded due to alignment specifier
*/
#pragma warning(disable: 4127) 
/* Silience!
1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Third-Party\Libraries\taskflow-3.8.0\taskflow\core\nonblocking_notifier.hpp(291,18): warning C4127: conditional expression is constant
*/
#pragma warning(disable: 4267) 
/* Silience!
1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Third-Party\Libraries\taskflow-3.8.0\taskflow\core\executor.hpp(1184,13): warning C4267: 'return': conversion from 'size_t' to 'int', possible loss of data
*/
// Frogman Engine uses /Wx and /W4. These lines of warning disablers are added by the developer of the engine.




// Feature macros for fine-tuning the performance of Taskflow at compile time
// 
// Disabled features by default:
// + TF_ENABLE_TASK_POOL       : enable task pool optimization
// + TF_ENABLE_ATOMIC_NOTIFIER : enable atomic notifier (required C++20)
//

#ifndef TF_DEFAULT_BOUNDED_TASK_QUEUE_LOG_SIZE 
/**
@def TF_DEFAULT_BOUNDED_TASK_QUEUE_LOG_SIZE

This macro defines the default size of the bounded task queue in Log2. 
Bounded task queue is used by each worker.
*/
  #define TF_DEFAULT_BOUNDED_TASK_QUEUE_LOG_SIZE 8
#endif

#ifndef TF_DEFAULT_UNBOUNDED_TASK_QUEUE_LOG_SIZE 
/**
@def TF_DEFAULT_UNBOUNDED_TASK_QUEUE_LOG_SIZE

This macro defines the default size of the unbounded task queue in Log2.
Unbounded task queue is used by the executor.
*/
  #define TF_DEFAULT_UNBOUNDED_TASK_QUEUE_LOG_SIZE 10
#endif


#include "core/executor.hpp"
#include "core/async.hpp"

/**
@dir taskflow
@brief root taskflow include dir
*/

/**
@dir taskflow/core
@brief taskflow core include dir
*/

/**
@dir taskflow/algorithm
@brief taskflow algorithms include dir
*/

/**
@dir taskflow/cuda
@brief taskflow CUDA include dir
*/

/**
@file taskflow/taskflow.hpp
@brief main taskflow include file
*/



/**
@def TF_VERSION 

@brief version of the %Taskflow (currently 3.8.0)

The version system is made of a major version number, a minor version number,
and a patch number:
  + TF_VERSION % 100 is the patch level
  + TF_VERSION / 100 % 1000 is the minor version
  + TF_VERSION / 100000 is the major version
*/
#define TF_VERSION 300800

/**
@def TF_MAJOR_VERSION

@brief major version of %Taskflow, which is equal to `TF_VERSION/100000`
*/
#define TF_MAJOR_VERSION TF_VERSION/100000

/**
@def TF_MINOR_VERSION

@brief minor version of %Taskflow, which is equal to `TF_VERSION / 100 % 1000`
*/
#define TF_MINOR_VERSION TF_VERSION/100%1000

/**
@def TF_PATCH_VERSION

@brief patch version of %Taskflow, which is equal to `TF_VERSION % 100`
*/
#define TF_PATCH_VERSION TF_VERSION%100



/**
@brief taskflow namespace
*/
namespace tf {

/**
@private
*/
namespace detail { }


/**
@brief queries the version information in a string format @c major.minor.patch

Release notes are available here: https://taskflow.github.io/taskflow/Releases.html
*/
constexpr const char* version() {
  return "3.8.0";
}


}  // end of namespace tf -----------------------------------------------------





#pragma warning(pop) // Frogman Engine uses /Wx and /W4. These lines of warning disablers are added by the developer of the engine.