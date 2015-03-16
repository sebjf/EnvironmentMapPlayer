package environmentmapplayer;

public final class MapParameters {

	public static int tile_width_in_pixels 	= 8;

	public static int max_mip_level			= 10; //mip level cannot be so high that the map has a higher resolution than the source
	public static int min_mip_level			= 3; //mip level cannot be so low that the map is smaller than the tile size

	public static int num_faces				= 8;

	public static int pixel_size_in_bytes 	= 4;

}
