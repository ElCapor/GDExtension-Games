target_include_directories(${PROJECT_NAME}
    PRIVATE
        "include"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/Coin.cpp
        src/CoinDashGame.cpp
        src/HUD.cpp
        src/Player.cpp
        src/Powerup.cpp
)

# we have to disable this warning about converting double to real_t or whatever
target_compile_options(${PROJECT_NAME} PRIVATE "/wd4244")