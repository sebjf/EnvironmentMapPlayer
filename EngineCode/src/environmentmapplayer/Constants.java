package environmentmapplayer;


public final class Constants {

	/* Map Properties */

	public static int tile_width_in_pixels 	= 8;
	public static int max_mip_level			= 11; //mip level cannot be so high that the map has a higher resolution than the source
	public static int min_mip_level			= 3;  //mip level cannot be so low that the map is smaller than the tile size
	public static int num_faces				= 6;
	public static int pixel_size_in_bytes 	= 4;
	public static int map_word_size_in_bytes= 384;

	/* Platform Properties */

	public static int burstSizeInBytes 		= 64;	// memory width - 384 for Maia, 192 for Isca, 64 for Isca single dimm

	public static int GetBurstsPerWord()
	{
		return map_word_size_in_bytes / burstSizeInBytes;
	}

	public static int GetTilesPerWord()
	{
		return 1;
	}

}
