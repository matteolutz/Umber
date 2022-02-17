set(PROJECT_NAME Umber)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "src/test/primes.ub"
)
source_group("" FILES ${no_group_source_files})

set(Headerdateien
    "src/Constants.h"
    "src/Context.fwd.h"
    "src/Context.h"
    "src/Error.h"
    "src/errors/ExpectedCharError.h"
    "src/errors/IllegalCharacterError.h"
    "src/errors/InvalidSyntaxError.h"
    "src/errors/RuntimeError.h"
    "src/Interpreter.h"
    "src/Lexer.h"
    "src/Node.h"
    "src/nodes/AccessorNode.h"
    "src/nodes/AccessorSetNode.h"
    "src/nodes/BinOpNode.h"
    "src/nodes/BreakNode.h"
    "src/nodes/CallNode.h"
    "src/nodes/ContinueNode.h"
    "src/nodes/DictNode.h"
    "src/nodes/ForNode.h"
    "src/nodes/FunctionDefNode.h"
    "src/nodes/IfNode.h"
    "src/nodes/ImportNode.h"
    "src/nodes/ListNode.h"
    "src/nodes/NumberNode.h"
    "src/nodes/ReturnNode.h"
    "src/nodes/StringNode.h"
    "src/nodes/UnaryOpNode.h"
    "src/nodes/VarAccessNode.h"
    "src/nodes/VarAssignNode.h"
    "src/nodes/VarDeclarationNode.h"
    "src/nodes/WhileNode.h"
    "src/Parser.h"
    "src/Position.h"
    "src/result/ParseResult.h"
    "src/result/RuntimeResult.h"
    "src/SymbolTable.fwd.h"
    "src/SymbolTable.h"
    "src/Token.h"
    "src/Umber.h"
    "src/utils/Utils.h"
    "src/Value.fwd.h"
    "src/Value.h"
    "src/values/BaseFunctionValue.h"
    "src/values/BuiltInFunction.h"
    "src/values/DictValue.h"
    "src/values/FunctionValue.h"
    "src/values/ListValue.h"
    "src/values/NumberValue.h"
    "src/values/StringValue.h"
)
source_group("Headerdateien" FILES ${Headerdateien})

set(Quelldateien
    "src/Context.cpp"
    "src/Error.cpp"
    "src/errors/ExpectedCharError.cpp"
    "src/errors/IllegalCharacterError.cpp"
    "src/errors/InvalidSyntaxError.cpp"
    "src/errors/RuntimeError.cpp"
    "src/Interpreter.cpp"
    "src/Lexer.cpp"
    "src/Node.cpp"
    "src/nodes/AccessorNode.cpp"
    "src/nodes/AccessorSetNode.cpp"
    "src/nodes/BinOpNode.cpp"
    "src/nodes/BreakNode.cpp"
    "src/nodes/CallNode.cpp"
    "src/nodes/ContinueNode.cpp"
    "src/nodes/DictNode.cpp"
    "src/nodes/ForNode.cpp"
    "src/nodes/FunctionDefNode.cpp"
    "src/nodes/IfNode.cpp"
    "src/nodes/ImportNode.cpp"
    "src/nodes/ListNode.cpp"
    "src/nodes/NumberNode.cpp"
    "src/nodes/ReturnNode.cpp"
    "src/nodes/StringNode.cpp"
    "src/nodes/UnaryOpNode.cpp"
    "src/nodes/VarAccessNode.cpp"
    "src/nodes/VarAssignNode.cpp"
    "src/nodes/VarDeclarationNode.cpp"
    "src/nodes/WhileNode.cpp"
    "src/Parser.cpp"
    "src/Position.cpp"
    "src/result/ParseResult.cpp"
    "src/result/RuntimeResult.cpp"
    "src/SymbolTable.cpp"
    "src/Token.cpp"
    "src/Umber.cpp"
    "src/utils/Utils.cpp"
    "src/Value.cpp"
    "src/values/BaseFunctionValue.cpp"
    "src/values/BuiltInFunction.cpp"
    "src/values/DictValue.cpp"
    "src/values/FunctionValue.cpp"
    "src/values/ListValue.cpp"
    "src/values/NumberValue.cpp"
    "src/values/StringValue.cpp"
)
source_group("Quelldateien" FILES ${Quelldateien})

set(ALL_FILES
    ${no_group_source_files}
    ${Headerdateien}
    ${Quelldateien}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Umber)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /Od;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /Od;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

