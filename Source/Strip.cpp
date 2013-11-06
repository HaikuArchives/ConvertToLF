#include <Application.h> 
#include <InterfaceKit.h> 
#include <StorageKit.h> 
#include <be/add-ons/tracker/TrackerAddon.h> 

#include <stdio.h> 
#include <string.h> 

bool StripFile(const char *FileName)
{
	if (FileName)
	{
		BFile f(FileName, B_READ_WRITE);
		if (f.InitCheck() == B_OK)
		{
			off_t Size;
			if (f.GetSize(&Size) == B_OK)
			{
				char *Buf = new char[Size+1];
				if (Buf)
				{
					f.Read(Buf, Size);
					Buf[Size] = 0;
					
					char *Out = Buf;
					for (char *In = Buf; *In; In++)
					{
						if (*In != '\r')
						{
							*Out = *In;
							Out++;
						}
					}
					
					*Out++ = 0; // NULL terminate
					
					Size = strlen(Buf);
					
					f.Seek(0, SEEK_SET);
					if (f.SetSize(0) == B_OK)
					{
						f.Write(Buf, Size);
						return true;
					}
				}
			}
		}
		else
		{
			printf("Couldn't open '%s'\n", FileName);
		}
	}
	
	return false;
}

void process_refs(entry_ref dir_ref, BMessage *msg, void *reserved)
{
	entry_ref ref;
	for (int i=0; msg->FindRef("refs", i, &ref) == B_OK; i++)
	{
		BEntry entry(&ref);

		BPath path;
		if (entry.GetPath(&path) == B_OK)
		{
			StripFile(path.Path());
		}
	}
}

int main(int args, char **arg)
{
	new BApplication("application/x-ConvertToLF");
	delete be_app;

	return 0;
}