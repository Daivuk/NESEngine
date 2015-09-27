palWorld1_sprites:  .incbin "assets/world1_sprites.pal"
tiles:              .incbin "assets/tiles.bin"
worldDataFile:      .incbin "assets/world.bin"
worldPalettes       = worldDataFile
worldHeader         = worldDataFile + 128
worldData           = worldDataFile + 128 + 5
