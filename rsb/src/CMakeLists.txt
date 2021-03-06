cmake_minimum_required(VERSION 2.6)

# --- generate protocol buffers
PROTOBUF_GENERATE(CPP PROTO_CPP_SOURCES PROTO_CPP_HEADERS
                      PROTOFILES ${CMAKE_CURRENT_SOURCE_DIR}/rct/proto/FrameTransform.proto
                      PROTOROOT ${CMAKE_CURRENT_SOURCE_DIR}/rct/proto/
                      INCLUDES ${RST_STABLE_PROTO_ROOT}
                      OUTPATH ${CMAKE_CURRENT_BINARY_DIR}/rct/)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# --- build instructions proto ---
ADD_LIBRARY(${RCT_NAME_PROTO} SHARED ${PROTO_CPP_SOURCES} ${PROTO_CPP_HEADERS})
TARGET_LINK_LIBRARIES(${RCT_NAME_PROTO} ${RST_LIBRARIES})
SET_TARGET_PROPERTIES(${RCT_NAME_PROTO} PROPERTIES VERSION ${RCT_VERSION} SOVERSION ${RCT_API_VERSION})
INSTALL(TARGETS ${RCT_NAME_PROTO} LIBRARY DESTINATION ${LIBDIR})
INSTALL(FILES ${PROTO_CPP_HEADERS} DESTINATION ${INCLUDEDIR}/rct)

# --- generate executable
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/core/src ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
ADD_LIBRARY(${RCT_NAME_RSB} SHARED rct/impl/TransformCommRsb.cpp rct/impl/TransformConverter.cpp)
TARGET_LINK_LIBRARIES(${RCT_NAME_RSB} ${RSB_LIBRARIES} ${RST_LIBRARIES} ${RCT_NAME} ${RCT_NAME_PROTO})
SET_TARGET_PROPERTIES(${RCT_NAME_RSB} PROPERTIES
                                 VERSION ${RCT_VERSION}
                                 SOVERSION ${RCT_API_VERSION})

# --- install target
INSTALL(TARGETS ${RCT_NAME_RSB}
        LIBRARY DESTINATION ${LIBDIR})
INSTALL(DIRECTORY . DESTINATION ${INCLUDEDIR}
          FILES_MATCHING
          PATTERN "./rct/impl/*.h"
)

