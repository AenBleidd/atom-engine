# Module names

# WindowManager module name
SET ( WM_MODULE "WindowManager" )

# AtomError module name
SET ( ATOMERROR_MODULE "AtomError" )

# GameFS module name
SET ( GAMEFS_MODULE "GameFS" )                        # Module which will be used by the engine
SET ( GAMEFS_FSMAN_MODULE "${GAMEFS_MODULE}_FSMan" )  # Module which will be used to build fsman utility

# FSMan module name
SET ( FSMAN_MODULE "FSMan" )
# SysVars module name
# Needed to identify some system variables
SET ( SYSVARS_MODULE "SysVars" )

# ResourceManager module name
# Needed to manage memory
SET ( RESOURCEMANAGER_MODULE "ResourceManager" )

# Main ancestor class for all engine
SET ( OATOMENGINE "AtomEngine" )