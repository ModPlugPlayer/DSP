#Build script of DSP Project
#Copyright (C) 2020 Volkan Orhan

#This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

#This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

#You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

cmake_minimum_required(VERSION 3.16)
project(DSP)

set(CMAKE_CXX_STANDARD 20)

file(GLOB DSP_SRC
    "src/*.hpp"
    "src/*.cpp"
)

file(GLOB COMMON_SRC
    "../Common/src/*.hpp"
    "../Common/src/*.cpp"
)

include_directories(../Common/src)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(DSP
    ${DSP_SRC}
    ${COMMON_SRC}
)
