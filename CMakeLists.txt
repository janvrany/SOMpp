cmake_minimum_required (VERSION 2.8.11)
project (SOMPP)

# set(CMAKE_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
# set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -fstack-protector-all")

add_subdirectory(omr)

set(COMPILER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/compiler")

set(COMPILER_SRC "${COMPILER_INCLUDE}/BytecodeGenerator.cpp"
		 "${COMPILER_INCLUDE}/ClassGenerationContext.cpp"
		 "${COMPILER_INCLUDE}/Disassembler.cpp"
		 "${COMPILER_INCLUDE}/Lexer.cpp"
		 "${COMPILER_INCLUDE}/MethodGenerationContext.cpp"
		 "${COMPILER_INCLUDE}/Parser.cpp"
		 "${COMPILER_INCLUDE}/SourcecodeCompiler.cpp")

set(MEMORY_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/memory")

set(MEMORY_SRC "${MEMORY_INCLUDE}/CopyingCollector.cpp"
               "${MEMORY_INCLUDE}/CopyingHeap.cpp"
               "${MEMORY_INCLUDE}/GenerationalCollector.cpp"
	       "${MEMORY_INCLUDE}/GenerationalHeap.cpp"
	       "${MEMORY_INCLUDE}/Heap.cpp"
	       "${MEMORY_INCLUDE}/MarkSweepCollector.cpp"
	       "${MEMORY_INCLUDE}/MarkSweepHeap.cpp"
	       "${MEMORY_INCLUDE}/OMRCollector.cpp"
	       "${MEMORY_INCLUDE}/OMRHeap.cpp")

set(MISC_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/misc")

set(MISC_SRC "${MISC_INCLUDE}/Timer.cpp")

set(INTERPRETER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/interpreter")

set(INTERPRETER_SRC "${INTERPRETER_INCLUDE}/Interpreter.cpp"
                    "${INTERPRETER_INCLUDE}/bytecodes.cpp")

set(PRIMITIVES_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/primitives")

set(PRIMITIVES_SRC "${PRIMITIVES_INCLUDE}/Array.cpp"
                   "${PRIMITIVES_INCLUDE}/Block.cpp"
		   "${PRIMITIVES_INCLUDE}/Class.cpp"
		   "${PRIMITIVES_INCLUDE}/Double.cpp"
		   "${PRIMITIVES_INCLUDE}/Integer.cpp"
		   "${PRIMITIVES_INCLUDE}/Method.cpp"
		   "${PRIMITIVES_INCLUDE}/Object.cpp"
		   "${PRIMITIVES_INCLUDE}/Primitive.cpp"
		   "${PRIMITIVES_INCLUDE}/SOMString.cpp"
		   "${PRIMITIVES_INCLUDE}/Symbol.cpp"
		   "${PRIMITIVES_INCLUDE}/System.cpp")

set(PRIMITIVES_CORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/primitivesCore")

set(PRIMITIVES_CORE_SRC "${PRIMITIVES_CORE_INCLUDE}/PrimitiveContainer.cpp"
                        "${PRIMITIVES_CORE_INCLUDE}/PrimitiveLoader.cpp")

set(VM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/vm")

set(VM_SRC "${VM_INCLUDE}/Shell.cpp"
           "${VM_INCLUDE}/Universe.cpp")

set(JIT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/jit")
	 
set(JIT_SRC # "${JIT_INCLUDE}/SOMppMethod.cpp"
            "${JIT_INCLUDE}/SOMppMethod_with_vm_state.cpp")

set(VM_OBJECTS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src/vmobjects")

set(VM_OBJECTS_SRC "${VM_OBJECTS_INCLUDE}/AbstractObject.cpp"
                   "${VM_OBJECTS_INCLUDE}/IntegerBox.cpp"
                   "${VM_OBJECTS_INCLUDE}/Signature.cpp"
                   "${VM_OBJECTS_INCLUDE}/VMArray.cpp"
                   "${VM_OBJECTS_INCLUDE}/VMBlock.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMClass.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMDouble.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMEvaluationPrimitive.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMFrame.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMInteger.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMInvokable.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMMethod.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMObject.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMPrimitive.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMString.cpp"
		   "${VM_OBJECTS_INCLUDE}/VMSymbol.cpp")

add_library(som_vm INTERFACE)
		 
target_sources(som_vm INTERFACE
  "${INTERPRETER_SRC}" "${VM_OBJECTS_SRC}" "${VM_SRC}" "${MEMORY_SRC}" "${JIT_SRC}"
  "${PRIMITIVES_CORE_SRC}"
  "${PRIMITIVES_SRC}"
  "${COMPILER_SRC}"
  "${MISC_SRC}"
)

target_link_libraries(som_vm INTERFACE jitbuilder omrcore omrvmstartup)

target_include_directories(som_vm INTERFACE
  "${OMR_GLUE_INCLUDE}" "${OMR_INCLUDE}" "${INTERPRETER_INCLUDE}" "${JIT_INCLUDE}"
  "${VM_INCLUDE}" "${VM_OBJECTS_INCLUDE}" "${MEMORY_INCLUDE}"
  "${PRIMITIVES_CORE_INCLUDE}"
  "${PRIMITIVES_INCLUDE}"
  "${COMPILER_INCLUDE}"
  "${MISC_INCLUDE}")

set(OMR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/omr/compiler"
                "${CMAKE_CURRENT_SOURCE_DIR}/omr/jitbuilder/release/include"
                "${CMAKE_CURRENT_SOURCE_DIR}/omr/gc"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/structs"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/startup"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/verbose"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/verbose/handler_standard"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/api"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/base"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/base/segregated"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/base/standard"
		"${CMAKE_CURRENT_SOURCE_DIR}/omrglue"
		"${CMAKE_CURRENT_SOURCE_DIR}/omr/gc/stats"
                "${CMAKE_CURRENT_SOURCE_DIR}/omr")
		 
set(OMR_GLUE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/omrglue")

set(OMR_GLUE_SRC "${OMR_GLUE_INCLUDE}/BytecodeHelper.cpp"
                 "${OMR_GLUE_INCLUDE}/CollectorLanguageInterfaceImpl.cpp"
		 "${OMR_GLUE_INCLUDE}/CompactSchemeFixupObject.cpp"
		 "${OMR_GLUE_INCLUDE}/ConfigurationLanguageInterfaceImpl.cpp"
		 "${OMR_GLUE_INCLUDE}/EnvironmentDelegate.cpp"
		 "${OMR_GLUE_INCLUDE}/EnvironmentLanguageInterfaceImpl.cpp"
		 "${OMR_GLUE_INCLUDE}/FrequentObjectsStats.cpp"
		 "${OMR_GLUE_INCLUDE}/FrequentObjectsStats.cpp"
		 "${OMR_GLUE_INCLUDE}/GlobalCollectorDelegate.cpp"
		 "${OMR_GLUE_INCLUDE}/LanguageVMGlue.c"
		 "${OMR_GLUE_INCLUDE}/MarkingDelegate.cpp"
		 "${OMR_GLUE_INCLUDE}/UtilGlue.c"
		 "${OMR_GLUE_INCLUDE}/ObjectIterator.cpp"
		 "${OMR_GLUE_INCLUDE}/ObjectModel.cpp"
		 "${OMR_GLUE_INCLUDE}/Profiling.cpp"
		 "${OMR_GLUE_INCLUDE}/StartupManagerImpl.cpp"
		 "${OMR_GLUE_INCLUDE}/VerboseManagerImpl.cpp")

set(OMR_JITBUILDER ON)
	       
add_library(omr_som_gc_glue INTERFACE)

set(OMR_GC_GLUE_TARGET "omr_som_gc_glue" CACHE
    STRING "The gc glue target, must be interface library" FORCE)

target_sources(omr_som_gc_glue
  INTERFACE
    ${OMR_GLUE_SRC})

target_link_libraries(omr_som_gc_glue
  INTERFACE
    omr_example_base
    som_vm)

target_include_directories(omr_som_gc_glue INTERFACE
   "${OMR_GLUE_INCLUDE}" "${OMR_INCLUDE}" "${VM_INCLUDE}")

add_definitions(-DDEFAULT_OMR_JIT_ON=true -DGC_TYPE=OMR_GARBAGE_COLLECTION) # -DUSE_TAGGING=false -DCACHE_INTEGER=false)
#  was OMR_GARBAGE_COLLECTION

add_executable(som
               "src/Main.cpp")

target_include_directories(som PRIVATE ${VM_INCLUDE}
                                       ${VM_OBJECTS_INCLUDE}
                                       ${COMPILER_INCLUDE}
				       ${MISC_INCLUDE}
				       ${INTERPRETER_INCLUDE}
				       ${PRIMITIVES_INCLUDE}
				       ${PRIMITIVES_CORE_INCLUDE}
				       ${MEMORY_INCLUDE}
				       ${OMR_GLUE_INCLUDE}
				       ${OMR_INCLUDE}
				       src/)

target_link_libraries(som omr_som_gc_glue omrgc som_vm)
