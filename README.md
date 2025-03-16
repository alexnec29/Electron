# Electron - Electronic Circuit Editor

## Description for CV
During the Introduction to Programming course, I worked on the **Electron project**, an electronic circuit editor designed to allow users to add, remove, rotate, and resize components, create connections between them, and test circuit validity. Components were loaded from text files in a PostScript format, with circuits being saved and loaded as text files. My role involved writing and testing code for circuit control and interaction using C/C++, troubleshooting technical challenges, and documenting the development process to track progress. This experience strengthened my programming skills, deepened my understanding of electronic circuits, and enhanced my ability to collaborate effectively in a small team setting.

## Overview
**Electron** is an electronic circuit editor built in **C++** using **Code::Blocks** and **graphics.h** for rendering.  
Developed in collaboration with a teammate, the project allows users to create and manipulate electronic circuits interactively.  

### Key Functionalities:
- **Add, remove, rotate, and resize electronic components**
- **Create valid circuit connections** (as cables, no current/voltage values)
- **Rename components** and assign values for reference
- **Zoom & rotate freely**
- **Drag the screen** for easier navigation
- **Customize colors via a settings menu**
- **Set cables in 3 distinct phases using right-click:**
  - Cutting middle
  - Reaching **X first, then Y**
  - Reaching **Y first, then X**
- **Supports PostScript-style component design files**

## Features
**Graphical Circuit Editor** – Interactive UI with `graphics.h`.  
**Flexible Component Handling** – Add, rotate, resize, rename.  
**Connection System** – Cables with structured routing.  
**File Support** – PostScript-based component design, save/load circuit files.  
**UI Customization** – Personalized colors via the menu.  

## Installation

### Prerequisites
- **Code::Blocks** with a **C++ compiler** (MinGW recommended)
- **WinBGIm (graphics.h)** installed
