// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/framework.hpp>




/*
Profile Guided Optimization: https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170

TO DO: 
1. variable table
                                 
																														  |--- int32 variable hash map
																														  |--- float64 variable hash map
																														  |--- FE::string variable hash map
std::unordered_map<Key: type name string, Value: std::unordered_map<Key: varaible name string, Value: varaible address>> -|--- FE::array variable hash map
																														  |--- std::list variable hash map
																														  |--- std::deque variable hash map
2. memory layout reflection for serialization  																		      |--- etc...
                                                                                      *
	                                                                                  |
	Shallower <----- Memory Hierarchy Depth ----> Deeper                      Lower Memory Address
                                                      |----------------|              |
	                                                  | - FE::string - |              |
	|----------------------------------|    ----------|FE::smart_ptr   |              |
	|  Target Entry Non-Trivial Object |    |         |length, capacity|              |
	|- member variables -              |    |         |----------------|              |
<<<<<<< HEAD
	|  FE::string m_raw_name --------------|----|                                         |
=======
	|  FE::string m_name --------------|----|                                         |
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
	                                           |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
													                                  |
																					  *
																					  
<<<<<<< HEAD
Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
=======
Memory Layer Traversal Order: Entry.FE::string m_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
																									|
												    |-----------------------------------------------|
												    |
												    |--> FE::string.length -> FE::string.capacity
																					   |
																					   |
									           |---------------------------------------|
											   |
											   |--> FE::vector.x -> FE::vector.y -> FE::vector.z
*/ 
class my_test_app : public FE::framework::application_base
{
public:
    my_test_app() = default;
    ~my_test_app() = default;

private:
    virtual bool set_up(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override
    {

        return true;
    }

	virtual int run(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override
    {
        FE_LOG("Successfully launched an application via the predefined main()!");
        return 0;
    }

	virtual void clean_up()override
    {

    }
};




CONFIGURE_A_FROGMAN_ENGINE_PROJECT()

return new my_test_app;

END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION()