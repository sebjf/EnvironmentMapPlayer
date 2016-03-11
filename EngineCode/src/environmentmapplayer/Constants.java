package environmentmapplayer;


public final class Constants {

	/* Map Properties */

	public static int tile_width_in_pixels 	= 8;
	public static int max_mip_level			= 11; //mip level cannot be so high that the map has a higher resolution than the source
	public static int min_mip_level			= 3;  //mip level cannot be so low that the map is smaller than the tile size
	public static int num_faces				= 6;
	public static int pixel_size_in_bytes 	= 4;
	public static int map_tile_stride_bytes = 384; //the number of bytes between two tiles

	/* Platform Properties */

	public static int burstSizeInBytes 		= 64;	// memory width - 384 for Maia, 192 for Isca, 64 for Isca single dimm

	public static int GetTileStrideInBursts()
	{
		return map_tile_stride_bytes / burstSizeInBytes;
	}

	public static int GetBurstsPerTile()
	{
		return (tile_width_in_pixels * tile_width_in_pixels * pixel_size_in_bytes) / burstSizeInBytes;
	}

	public static int GetTilesPerWord()
	{
		return 1;
	}

	/* Graph properties */

	// The packing factor for the memory samples (once de-multiplexed)
	public static int burstPackingFactor = 3;

	// The packing factor for the memory commands
	public static int memoryPackingFactor = 4;

	// The packing factor for the pixel commands
	public static int pixelPackingFactor 	= 12;

	//number of pixel sample commands queued up at a time. As these are generated at a higher rate than they are output, this number will always be queued.
	//it should be large enough to overcome the latency and bandwidth bottleneck of the lmem. The optimal size is 13x the size of the map sample buffers.
	public static int pixelCommandDepth = 4096;

	public static int memoryCommandDepth = 4096;

	public static int memorySampleDepth = 3548;

	public static int pixelDepth = 2048;

}
