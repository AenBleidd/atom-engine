# Module names

# WindowManager module name
SET ( WM_MODULE "WindowManager" )

# AtomError module name
# Deprecated: delete
SET ( ATOMERROR_MODULE "AtomError" )

# AtomLog module name
SET ( ATOMLOG_MODULE "AtomLog" )

# GameFS module name
# Deprecated: delete
SET ( GAMEFS_MODULE "GameFS" )                        # Module which will be used by the engine
SET ( GAMEFS_FSMAN_MODULE "${GAMEFS_MODULE}_FSMan" )  # Module which will be used to build fsman utility

# AtomFS module name
SET ( ATOMFS_MODULE "AtomFS" )

# FSMan module name
SET ( FSMAN_MODULE "FSMan" )
# SysVars module name
# Needed to identify some system variables
SET ( SYSVARS_MODULE "SysVars" )

# ResourceManager module name
# Needed to manage memory
SET ( RESOURCEMANAGER_MODULE "ResourceManager" )

# Test modules
SEt ( TESTS_MODULE "AtomEngineTests" )