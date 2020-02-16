#if defined _tf2_wearables_included
  #endinput
#endif

native int TF2_EquipWearable(int client, const char[] classname, int index, int level = 1, int quality = 1);
native int TF2_EquipViewmodel(int client, char[] model = "");

#if !defined REQUIRE_PLUGIN
public void __pl_tf2_wearables_SetNTVOptional()
{
	MarkNativeAsOptional("TF2_EquipWearable");
	MarkNativeAsOptional("TF2_EquipViewmodel");
}
#endif

public SharedPlugin __pl_tf2_wearables =
{
	name = "tf2-wearables",
	file = "tf2-wearables.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};
