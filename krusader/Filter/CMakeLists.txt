set(Filter_SRCS
    filtersettings.cpp
    advancedfilter.cpp
    generalfilter.cpp
    filtertabs.cpp
    filterdialog.cpp)

add_library(Filter STATIC ${Filter_SRCS})

target_link_libraries(Filter
    Dialogs
    KF5::Codecs
    KF5::ConfigCore
    KF5::I18n
    KF5::IconThemes
    KF5::WidgetsAddons
)
