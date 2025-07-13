# NSMBW-BlueToadPhysics
Port SM3DW blue toad physics to NSMBW.
## Usage
1. Place **BToadPhysics.bin** into Code (or NewerRes) folder of your mod.  
2. Add below entry to the riivolution xml file.  
```
		<!-- Blue Toad Physics Patch -->
		<memory offset="0x80005e18" valuefile="Code/BToadPhysics.bin" /> <!-- If you placed the .bin file into NewerRes, change this entry! -->
		<memory offset="0x8005DF0C" value="4BFA7F0C" original="3800000f" />
```
## Compile
This patch needs these:
- Python (recommend 3.8.10)  
- [kamek](https://github.com/Treeki/Kamek/tree/414b2b404a29c8c745140d9555c81d29273b16db)  
- [NXP CodeWarrior Special Edition for MPC55xx/MPC56xx v2.10](http://cache.nxp.com/lgfiles/devsuites/PowerPC/CW55xx_v2_10_SE.exe?WT_TYPE=IDE%20-%20Debug,%20Compile%20and%20Build%20Tools&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=exe&WT_ASSET=Downloads&fileExt=.exe)  
  
For information on how to use CodeWarrior, see Kamek's ReadMe.  
When you finished setup of kamek, run the build.bat.
