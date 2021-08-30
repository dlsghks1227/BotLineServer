# BotLineServer

## Introduction

자율주행체 및 Edge 서버를 제어 및 프로파일링하기 위한 중앙 서버 제작

## Requirements
- Intergrated Development Environment(IDE): Visual Studio 2019 Community
- Programming language: C++
- Graphics API: DirectX 11
	- User interface libaray: imgui, implot

## File structure
```
Server
├── Component
│    ├── Processing
│    │    ├── JetbotProcessingComponent.cpp
│    │    ├── JetbotProcessingComponent.h
│    │    ├── WebProcessingComponent.cpp
│    │    ├── WebProcessingComponent.h
│    │    ├── XavierProcessingComponent.cpp
│    │    └── XavierProcessingComponent.h
│    ├── BaseProcessingComponent.h
│    ├── NetworkComponent.cpp
│    └── NetworkComponent.h
├── DirectX
│    ├── DeviceResources.cpp
│    └── DeviceResources.h
├── imgui
│    ├── implot
│    │    └── ...
│    └── ...
├── Object
│    ├── BotLineObject.h
│    ├── JetbotObject.cpp
│    ├── JetbotObject.h
│    ├── WebObject.cpp
│    ├── WebObject.h
│    ├── XavierObject.cpp
│    └── XavierObject.h
├── UI
│    ├── Log.cpp
│    ├── Log.h
│    ├── UIBase.h
│    ├── UIManager.cpp
│    └── UIManager.h
├── Util
│    ├── Network
│    │    ├── MemoryBitStream.cpp
│    │    ├── MemoryBitStream.h
│    │    ├── Packet.h
│    │    ├── SocketAddress.h
│    │    ├── UDPSocket.cpp
│    │    └── UDPSocket.h
│    ├── Object
│    │    ├── Object.h
│    │    ├── ObjectCollection.cpp
│    │    └── ObjectCollection.h
│    ├── Component.cpp
│    ├── NetworkException.h
│    ├── SharedContext.h
│    └── Timer.h
├── Sources
│    ├── BotLine.cpp
│    ├── BotLineServer.cpp
│    └── pch.cpp
└── Headers
     ├── BotLine.h
     ├── BotLineServer.h
     ├── framework.h
     ├── pch.h
     ├── Resource.h
     └── targetver.h
```
