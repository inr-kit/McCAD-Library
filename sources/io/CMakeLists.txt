add_library(STEPReader stepreader.cpp)
install (TARGETS STEPReader DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(STEPReader_impl stepreader_impl.cpp)
install (TARGETS STEPReader_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(STEPWriter stepwriter.cpp)
install (TARGETS STEPWriter DESTINATION ${PROJECT_BINARY_DIR}/bin)

add_library(STEPWriter_impl stepwriter_impl.cpp)
install (TARGETS STEPWriter_impl DESTINATION ${PROJECT_BINARY_DIR}/bin)
