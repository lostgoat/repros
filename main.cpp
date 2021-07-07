/**
 * Repro for a funky bluez + object manager behaviour
 */

#include <gio/gio.h>

// Toggle this to switch between a working interface and a borked one (bluez)
static bool g_bUseUdisks = false;

const gchar *GetObjectManagerName()
{
	if ( g_bUseUdisks )
		return "org.freedesktop.UDisks2";
	return "org.bluez";
}

const gchar *GetObjectManagerPath()
{
	if ( g_bUseUdisks )
		return "/org/freedesktop/UDisks2";
	return "/";
}

static void DebugDumpObjectManager(GDBusObjectManager *pManager)
{
	g_print("Object manager: %s\n", g_dbus_object_manager_get_object_path(pManager));

	GList *pObjects = g_dbus_object_manager_get_objects(pManager);
	for (GList *iObject = pObjects; iObject != nullptr; iObject = iObject->next)
	{
		GDBusObject *pObject = G_DBUS_OBJECT(iObject->data);

		g_print("    Object: %s\n", g_dbus_object_get_object_path(G_DBUS_OBJECT(pObject)));

		GList *pInterfaces = g_dbus_object_get_interfaces(G_DBUS_OBJECT(pObject));
		for (GList *iInterface = pInterfaces; iInterface != nullptr; iInterface = iInterface->next)
		{
			GDBusInterface *pInterface = G_DBUS_INTERFACE(iInterface->data);
			GDBusInterfaceInfo *pInterfaceInfo = g_dbus_interface_get_info(pInterface);

			g_print("        Interface: %p\n", pInterface);
			g_print("            - is_interface: %d\n", G_IS_DBUS_INTERFACE(iInterface->data));
			g_print("            - info: %p\n", pInterfaceInfo);
			g_print("            - name %s\n", pInterfaceInfo ? pInterfaceInfo->name : "error");
		}
		g_list_free_full(pInterfaces, g_object_unref);
	}
	g_list_free_full(pObjects, g_object_unref);
}

void OnObjectManager(GObject *pObject, GAsyncResult *pResult, gpointer pUserData)
{
	GError *pError = nullptr;

	GDBusObjectManager *pManager = g_dbus_object_manager_client_new_for_bus_finish(pResult, &pError);
	if (!pManager || pError)
	{
		g_printerr("error: %s", pError->message);
		g_error_free(pError);
	}

	DebugDumpObjectManager(pManager);
	g_object_unref(pManager);

	exit(0);
}

int main()
{
	GMainLoop *pLoop = g_main_loop_new(nullptr, FALSE);

	g_dbus_object_manager_client_new_for_bus(G_BUS_TYPE_SYSTEM, G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
											 GetObjectManagerName(), GetObjectManagerPath(), nullptr, nullptr, nullptr, nullptr,
											 OnObjectManager, nullptr);

	g_main_loop_run(pLoop);

	return 0;
}
