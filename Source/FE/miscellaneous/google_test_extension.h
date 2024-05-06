#ifndef _FE_GOOGLE_TEST_EXTENSON_H_
#define _FE_GOOGLE_TEST_EXTENSON_H_
// Copyright 2008, Google Inc. All rights reserved. Google Test is Google's intellectual property.


#include <FE/core/macros/macro_definitions.h>


// Macros for Google-Testing Frogman Engine classes and structures
// Do not put namespaces in front of a class name, nor put this macro inside any namespaces.
#define ENABLE_TEST_FOR(test_subject_class_name_p) class sample_##test_subject_class_name_p##_instance_Test

// Do not put namespaces in front of a class name, nor put this macro inside any namespaces.
#define ALLOW_ITS_FRIEND_TO_TEST(test_subject_class_name_p) friend class sample_##test_subject_class_name_p##_instance_Test

// Do not put namespaces in front of a class name, nor put this macro inside any namespaces.
#define TEST_FROGMAN_ENGINE(test_subject_class_name_p, test_case_p) TEST_F(sample_##test_subject_class_name_p, test_case_p)


#ifdef _MSVC_


#define DECLARE_TEST_CLASS(namespace_p, test_subject_class_name_p) \
class sample_##test_subject_class_name_p : public testing::Test \
{ \
    friend class sample_##test_subject_class_name_p##_instance_Test; \
protected: \
    namespace_p##::##test_subject_class_name_p* m_test_subject; \
\
    void SetUp() override \
    { \
        this->m_test_subject = new namespace_p##::##test_subject_class_name_p; \
    } \
\
    void TearDown() override \
    { \
        delete m_test_subject; \
        this->m_test_subject = nullptr; \
    } \
}


#elif defined(_CLANG_) || defined(_GNUC_)


#define DECLARE_TEST_CLASS(namespace_p, test_subject_class_name_p) \
class sample_##test_subject_class_name_p : public testing::Test \
{ \
    friend class sample_##test_subject_class_name_p##_instance_Test; \
protected: \
    namespace_p::test_subject_class_name_p* m_test_subject; \
\
    void SetUp() override \
    { \
        this->m_test_subject = new namespace_p::test_subject_class_name_p; \
    } \
\
    void TearDown() override \
    { \
        delete m_test_subject; \
        this->m_test_subject = nullptr; \
    } \
}


#endif


#endif
