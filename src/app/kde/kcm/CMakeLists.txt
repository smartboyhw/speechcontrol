project(SpeechControl-KControlModule)

find_package (KDE4 REQUIRED)

## Includes
include (KDE4Defaults)
include (MacroLibrary)

## Variables
set(SPCHCNTRL_KCM_DEFINITIONS "-DQT_NO_CAST_FROM_ASCII -DQT_NO_KEYWORDS")
set(SPCHCNTRL_KCM_INCLUDE_DIRS ${KDE4_INCLUDES})
set(SPCHCNTRL_KCM_SRCS )

## Add subdirectory
add_subdirectory(ui)
add_subdirectory(src)
add_subdirectory(data)