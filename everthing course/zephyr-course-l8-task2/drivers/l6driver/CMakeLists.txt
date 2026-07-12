if (CONFIG_L6DRIVER)
target_sources(app PRIVATE l6driver.c)
target_include_directories(app PRIVATE .)
endif ()