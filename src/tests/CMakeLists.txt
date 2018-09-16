set(PatternFilterTest_SRCS
   ../patternfilter.cpp
   patternfiltertest.cpp)


set(PatternFilterTest_MOC_HDRS
   patternfiltertest.h
)

qt4_wrap_cpp(PatternFilterTest_SRCS ${PatternFilterTest_MOC_HDRS})
set(TEST_LIBRARIES ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} )

add_executable(PatternFilterTest ${PatternFilterTest_SRCS})
target_link_libraries(PatternFilterTest ${TEST_LIBRARIES})
add_test(NAME PatternFilterTest COMMAND PatternFilterTest)
