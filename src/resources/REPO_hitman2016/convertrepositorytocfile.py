#!/usr/bin/env python3

import json

types = [
	"melee",
	"pistol",
	"tool",
	"explosives",
	"sniperrifle",
	"assaultrifle",
	"shotgun",
	"distraction",
	"poison",
	"smg",
	"questitem",
	"INVALID_CATEGORY_ICON"
]

melee_guid_name = []
pistol_guid_name = []
tool_guid_name = []
explosives_guid_name = []
sniperrifle_guid_name = []
assaultrifle_guid_name = []
shotgun_guid_name = []
distraction_guid_name = []
poison_guid_name = []
smg_guid_name = []
questitem_guid_name = [] #essentials for quests
INVALID_CATEGORY_ICON_guid_name = []

"""
Warning, this script is an absolute abomination, however so is json
If you can fix it, i accept prs :)


Some quest items will also be included in other categories, like octane booster
its a quest item, however we also want it in explosives
"""

with open("000000000000016f.ini", "r") as f1:
	repo1 = json.load(f1)
with open("000000000000050b.ini", "r") as f2:
	repo2 = json.load(f2)

for repo in [repo1,repo2]:
	for value in repo:
		dupfound = 0
		for k in [	questitem_guid_name,
				melee_guid_name,
				pistol_guid_name,
				tool_guid_name,
				explosives_guid_name,
				sniperrifle_guid_name,
				assaultrifle_guid_name,
				shotgun_guid_name,
				distraction_guid_name,
				poison_guid_name,
				smg_guid_name,
				INVALID_CATEGORY_ICON_guid_name]:
			for v in k:
				if v[1] == value["ID_"]:
					dupfound = 1
					break
			if dupfound == 1:
				break
		if dupfound == 1:
			continue
		
		cn = ""
		if "CommonName" in value:
			if value["CommonName"] == "":
				cn = "NONAME:"+value["ID_"][0:8]
			elif value["CommonName"] == "\n      ": #there is one buggy item
				cn = value["Title"] #hackworkaround as it seems to be the only case
			else:
				cn = value["CommonName"]
		# has no name, probably garbage
		else:
			continue
		#print(value["ID_"] + " :: " + cn)
		
		#misc skip 1
		if "archme" in cn.lower() or 	\
			"archetype" in cn.lower() or \
			cn == "RemoteExplosiveDuck Bundle":
			continue
			
		if "Description_LOC" in value:
			if value["Description_LOC"] == "actor_description":
				continue

		if "Description" in value:
			if value["Description"].lower().startswith("outfit"):
				continue

		if cn.lower().startswith("elusivetarget_") or \
			cn.lower().startswith("char_") or \
			cn.lower().startswith("noel_unique_") or \
			("IsHitmanSuit" in value and value["IsHitmanSuit"] == True) or \
			value["ID_"] == "95949c3a-8882-4528-bad5-ad1da3cee57b" or \
			value["ID_"] == "fbedf9eb-287e-4b3f-b4d7-d406979dc55f":
			continue
		
		if "ModifierType" in value:
			if value["ModifierType"] == "MODIFIER_POINTSHOOTING":
				continue
			if value["ModifierType"] == "MODIFIER_RATEOFFIRE":
				continue
		
		if value["ItemType"] == "eDetonator" and 							\
				not value["InventoryCategoryIcon"] == "tool" and 			\
				not value["InventoryCategoryIcon"] == "distraction" or 		\
				cn == "Mine Disarm Device" or 								\
				value["InventoryCategoryIcon"] == "remote":
			continue
		
		if value["InventoryCategoryIcon"].lower() == "questitem" or \
			cn.lower().startswith("key") or \
			"questitems" in value["Name_LOC"].lower() or \
			value["ItemType"] == "eOther_Keycard_A" or \
			cn == "Marrakech_ExitKey_EmbassyStaffCar" or \
			cn == "Lumumba Memory Stick" or \
			cn == "Poisonable_VodkaGlass" or \
			cn == "Pilsner_Phone_B" or \
			cn == "Marrakech_ExitKey_ArmyTruck" or \
			cn == "Marrakech_ExitKey_DeliveryTruck" or \
			cn == "Marrakesh_Key_MaintenanceRoom" or \
			cn == "Bosco Car Key" or \
			cn == "Game Changer Safe      "  or \
			cn == "Key (Mansion Masterkey)" or \
			cn == "Bosco Trailer key" or \
			cn.lower().startswith("poisonable_"):
			questitem_guid_name.append(["questitem", value["ID_"], cn])
			continue
			
		if value["ItemType"] == "eCC_Bottle" and 					\
			not value["CommonName"] == "Rat Poison" and	\
			not value["InventoryCategoryName"] == "explosive":
			continue
			
		# misc skip 2
		if "suitcase" in cn.lower() or \
			"container" in value["InventoryCategoryIcon"].lower() or \
			"flash light" in cn.lower():
			continue
		
		#the rest
		if "melee" in value["InventoryCategoryIcon"].lower():
			melee_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue

		if "pistol" in value["InventoryCategoryIcon"].lower():
			pistol_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "tool" in value["InventoryCategoryIcon"].lower():
			tool_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "sniperrifle" in value["InventoryCategoryIcon"].lower():
			sniperrifle_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "assaultrifle" in value["InventoryCategoryIcon"].lower():
			assaultrifle_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "shotgun" in value["InventoryCategoryIcon"].lower():
			shotgun_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "distraction" in value["InventoryCategoryIcon"].lower():
			distraction_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "poison" in value["InventoryCategoryIcon"].lower():
			poison_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "suitcase" in value["InventoryCategoryIcon"].lower():
			suitcase_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "smg" in value["InventoryCategoryIcon"].lower():
			smg_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
			
		if "explosives" in value["InventoryCategoryIcon"].lower():
			explosives_guid_name.append([value["InventoryCategoryIcon"].lower(), value["ID_"], cn])
			continue
		
		#print(value["ID_"] + " :: " + cn + " UNHANDLED")
		# Mostly ammo boxes
		INVALID_CATEGORY_ICON_guid_name.append(["INVALID_CATEGORY_ICON", value["ID_"], cn])


"""
for k in [	questitem_guid_name,
			melee_guid_name,
			pistol_guid_name,
			tool_guid_name,
			explosives_guid_name,
			sniperrifle_guid_name,
			assaultrifle_guid_name,
			shotgun_guid_name,
			distraction_guid_name,
			poison_guid_name,
			smg_guid_name,
			INVALID_CATEGORY_ICON_guid_name]:
	for v in k:
		print(v)
"""

#check thrown outs
"""
for repo in [repo,repo2]:
	for value in repo:
		dupfound = 0
		for k in [	questitem_guid_name,
				melee_guid_name,
				pistol_guid_name,
				tool_guid_name,
				explosives_guid_name,
				sniperrifle_guid_name,
				assaultrifle_guid_name,
				shotgun_guid_name,
				distraction_guid_name,
				poison_guid_name,
				smg_guid_name,
				INVALID_CATEGORY_ICON_guid_name]:
			for v in k:
				if v[1] == value["ID_"]:
					dupfound = 1
					break
			if dupfound == 1:
				break
		if dupfound == 0:
			cn = ""
			if "CommonName" in value:
				if value["CommonName"] == "":
					cn = "NONAME:"+value["ID_"][0:8]
				elif value["CommonName"] == "\n      ": #there is one buggy item
					cn = value["Title"] #hackworkaround as it seems to be the only case
				else:
					cn = value["CommonName"]
				#for sure not items
				
				if "archme" in cn.lower() or 	\
					"archetype" in cn.lower() or \
					cn == "RemoteExplosiveDuck Bundle":
					continue
					
				if "Description_LOC" in value:
					if value["Description_LOC"] == "actor_description":
						continue

				if "Description" in value:
					if value["Description"].lower().startswith("outfit"):
						continue

				if cn.lower().startswith("elusivetarget_") or \
					cn.lower().startswith("char_") or \
					cn.lower().startswith("noel_unique_") or \
					("IsHitmanSuit" in value and value["IsHitmanSuit"] == True) or \
					value["ID_"] == "95949c3a-8882-4528-bad5-ad1da3cee57b" or \
					value["ID_"] == "fbedf9eb-287e-4b3f-b4d7-d406979dc55f":
					continue
				
				if "ModifierType" in value:
					if value["ModifierType"] == "MODIFIER_POINTSHOOTING":
						continue
					if value["ModifierType"] == "MODIFIER_RATEOFFIRE":
						continue
				
				#whats left over
				print("->"+value["ID_"]+"<- "+cn)
			else:
				continue
		else:
			continue
"""

out = open("Repository2016.c", "w")

out.write("#ifndef REPOSITORY2016_C\n")
out.write("#define REPOSITORY2016_C\n")
out.write("//This is autogenerated by convertrepositorytocfile.py!\n")
out.write("typedef struct {\n\tconst GUID* const guid;\n\tconst char* const name;\n} guid_name_struct;\n\n")


for k in [	questitem_guid_name,
			melee_guid_name,
			pistol_guid_name,
			tool_guid_name,
			explosives_guid_name,
			sniperrifle_guid_name,
			assaultrifle_guid_name,
			shotgun_guid_name,
			distraction_guid_name,
			poison_guid_name,
			smg_guid_name,
			INVALID_CATEGORY_ICON_guid_name]:
	longest_guid = 0
	longest_name = 0
	total = 0
	for v in k:
		if (len(v[1]) > longest_guid):
			longest_guid = len(v[1])
		if (len(v[2]) > longest_name):
			longest_name = len(v[2])
		#out.write("const char "+v[0]+str(total)+"guid[] = \""+v[1]+"\";\n")
		#print(v[1])
		news = v[1].replace("-", "")
		news2 = "{"+"0x"+news[0:8]+",0x"+news[8:12]+",0x"+news[12:16]+",0x"+news[16:18]+",0x"+news[18:20]+",0x"+news[20:22]+",0x"+news[22:24]+",0x"+news[24:26]+",0x"+news[26:28]+",0x"+news[28:30]+",0x"+news[30:32]+"}"
		out.write("const GUID "+v[0]+str(total)+"guid = "+news2+";\n")
		out.write("const char "+v[0]+str(total)+"name[] = \""+v[2]+"\";\n")
		total += 1

	out.write("const char "+v[0]+"[] = \""+v[0]+"\";\n")
	out.write("#define "+v[0]+"ARRLEN "+str(total)+"\n")
	out.write("guid_name_struct "+v[0]+"_guid_name["+v[0]+"ARRLEN] = {\n")
		
	tmpcnt = 0
	for v in k:
		#print(v[1]) #guid
		#print(v[2]) #name
		out.write("\t(const GUID* const)&"+v[0]+str(tmpcnt)+"guid, (const char* const)&"+v[0]+str(tmpcnt)+"name,\n")
		tmpcnt += 1

	out.write("};\n\n")

out.write("\n")


out.write("typedef enum {\n")
for k in types:
	out.write("\tc_"+k+",\n")
out.write("} e_class;\n\n")

out.write("""
typedef struct {
	e_class class_enum;
	const guid_name_struct* const itemrepo;
	size_t len;
	const char* const name;
} itemrepoinfo;\n""")

out.write("#define allitemreposARRLEN "+str(len(types))+"\n")
out.write("itemrepoinfo air[allitemreposARRLEN] = {\n")
for k in types:
	out.write("\t{c_"+k+", (const guid_name_struct* const)&"+k+"_guid_name, "+k+"ARRLEN, (const char* const)&"+k+"},\n")
out.write("};\n\n\n")

out.write("//Used to find class from guid\n")
out.write("""#define LOOPALLITEMS 													\\
	for (size_t iter = 0; iter < allitemreposARRLEN; iter++) {			\\
		itemrepoinfo* currentclass = &air[iter]; 						\\
		for (size_t iter2 = 0; iter2 < air[iter].len; iter2++) {		\\
			const guid_name_struct* item = &air[iter].itemrepo[iter2];	\\

#define LOOPALLITEMSEND	}}

#define GETPTRTOCLASSARRAY(class) 										\\
	for (size_t iter = 0; iter < allitemreposARRLEN; iter++) {			\\
		if (air[iter].class_enum != class)								\\
			continue;													\\
		size_t itemarrlen = air[iter].len;								\\
		/*const char* itemtypename = air[iter].name;*/					\\
		const guid_name_struct* itemrepo = air[iter].itemrepo; 			\\

#define GETPTRTOCLASSARRAYEND break; }\n\n\n""")
out.write("#endif\n")
out.close()

