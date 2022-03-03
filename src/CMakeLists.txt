#add_subdirectory(headless)

target_sources(NeuralPi PRIVATE
        CabSim.h
	PluginEditor.cpp
	PluginEditor.h
	PluginProcessor.cpp
	PluginProcessor.h
)

#target_precompile_headers(NeuralPi PRIVATE pch.h)
