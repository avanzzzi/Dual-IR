#add_subdirectory(headless)

target_sources(Dual-IR PRIVATE
        CabSim.h
	PluginEditor.cpp
	PluginEditor.h
	PluginProcessor.cpp
	PluginProcessor.h
)

#target_precompile_headers(Dual-IR PRIVATE pch.h)
