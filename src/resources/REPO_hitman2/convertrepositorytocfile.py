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
	"suitcase",
	"distraction",
	"poison",
	"smg",
	"questitem",
	"INVALID_CATEGORY_ICON",
#	"ignored",
]

melee_guid_name = []
pistol_guid_name = []
tool_guid_name = []
explosives_guid_name = []
sniperrifle_guid_name = []
assaultrifle_guid_name = []
shotgun_guid_name = []
suitcase_guid_name = []
distraction_guid_name = []
poison_guid_name = []
smg_guid_name = []
questitem_guid_name = [] #essentials for quests
INVALID_CATEGORY_ICON_guid_name = []
#ignored_guid_name = []

"""
Warning, this script is an absolute abomination, however so is json
If you can fix it, i accept prs :)


Some quest items will also be included in other categories, like octane booster
its a quest item, however we also want it in explosives
"""

with open("00204d1afd76ab13.REPO", "r") as f:
	repo = json.load(f)

for value in repo:
	##Verify claim below!
	#It seems every weapon/item we want has at least a commonname field
	cn = ""
	if "CommonName" in value:
		if value["CommonName"] == "":
			cn = "NONAME:"+value["ID_"][0:8]
		elif value["CommonName"] == "\n      ": #there is one buggy item
			cn = value["Title"] #hackworkaround as it seems to be the only case
		else:
			cn = value["CommonName"]
	else:
		continue

	#print(value["ID_"]+"\n  ->"+cn)

	#Ignore outfits
	if "Description" in value:
		if value["Description"].lower().startswith("outfit"): #"outfit" in 
			continue

	if "IsHitmanSuit" in value:
		continue

	if "ElusiveTarget" in value["CommonName"]:
		continue

	#Special cases
	if value["CommonName"] == "RemoteExplosiveDuck Bundle":
		continue

	#Ignore ghost mode stuff - needs to go first otherwise gmi ends up in questitems
	if "ghost" in value["CommonName"].lower() and \
		not "sieger" in value["CommonName"].lower(): #the sniper is the only non ghost mode item with the string ghost in it
		continue

	#crash-y items
	if "archme" in cn.lower() or 	\
		"archetype" in cn.lower() or \
		"selfie" in cn.lower():
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
		continue

	#print(cn + " accepted\n")

	#map completion items
	#janus intel, bank data code etc... find them all!
	#Tool_McGuffin==bank datacore
	if "intelitem" in cn.lower() or 						\
		"Tool_McGuffin" in value["CommonName"] or 			\
		"Cassidy Video Tape" in value["CommonName"] or 		\
		"Constant Photography" in value["CommonName"] or 	\
		"Janus Letter" in value["CommonName"] or 			\
		"Janus Memoir" in value["CommonName"] or 			\
		"Microfilm Skunk" in value["CommonName"]:
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
		continue


	#handle ignored items
	#general detonators for anything
	#lockpick==tool;coins=distraction
	#mine disarm device needs special treatment too
	#if "ItemType" in value:
	if value["ItemType"] == "eDetonator" and 							\
			not value["InventoryCategoryIcon"] == "tool" and 			\
			not value["InventoryCategoryIcon"] == "distraction" or 		\
			cn == "Mine Disarm Device" or 								\
			value["InventoryCategoryIcon"] == "remote":
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
		continue

	#items important for quests
	#in yolo mode we replace those too
	#if "HolsterAbility" in value:
	if 	value["HolsterAbility"] == "eHolsterQuestStorage" or 	\
		value["InventoryCategoryIcon"] == "QuestItem" or 		\
		value["InventoryCategoryName"] == "QuestItem" or 		\
		value["InventoryCategoryIcon"] == "questitem" or 		\
		value["InventoryCategoryName"] == "questitem" or 		\
	 	value["InventoryCategoryIcon"] == "key" or 				\
		value["InventoryCategoryName"] == "key" or 				\
		value["InventoryCategoryIcon"] == "USBStick" or 		\
		value["InventoryCategoryName"] == "USBStick" or 		\
		value["ActionRewardType"] == "AR_QuestItem" or 			\
		value["ItemType"] == "eOther_Keycard_A" or 				\
		"Chloroform Flask" in value["CommonName"] or 			\
		"Exploding_Watch_Battery" in value["CommonName"] or 	\
		"Key (Cultist Apartment)" in value["CommonName"] or 	\
		"QuestItem_" in value["CommonName"]:
		questitem_guid_name.append(["questitem", value["ID_"], cn])
		#questitems we might want to include for fun
		if value["InventoryCategoryIcon"] == "explosive" or		\
			value["InventoryCategoryName"] == "explosive":
			explosives_guid_name.append(["explosives", value["ID_"], cn])
		continue

	#examples for above
	#Colombia Lethal Poison Frog "InventoryCategoryName":"poison",
	#Letterbomb Remote Explosive "InventoryCategoryName":"explosive",
	#"Melee Burial Dagger " "InventoryCategoryName":"melee",
	#"Nitroglycerin" "InventoryCategoryName":"explosive",
	#"Octane Booster" "InventoryCategoryName":"explosive",
	#"QuestItem_CannonBall" "InventoryCategoryName":"tool",

	#get rid off special dartguns
	if "(Dartgun) Cure I" in cn or 	\
		"(Dartgun) Sick" in cn or 	\
		"(Dartgun) Tracker" in cn:
		continue
	
	#misc objects
	#explosive==nitro,octane;rat poison==if we check poison it will miss out sniperonly ignores
	#if "ItemType" in value:
	if value["ItemType"] == "eCC_Bottle" and 					\
		not value["InventoryCategoryName"] == "explosive" and	\
		not value["CommonName"] == "Rat Poison":
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
		continue

	#containers are not meant to be used in this way
	#while looking fun, its not that fun
	if "container" in value["InventoryCategoryIcon"].lower():
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
		continue

	#pointless items in my opinion
	if "outbreak" in cn.lower():
		#ignored_guid_name.append(["ignored", value["ID_"], cn])
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
		
	INVALID_CATEGORY_ICON_guid_name.append(["INVALID_CATEGORY_ICON", value["ID_"], cn])


#I am not sure what this file does
with open("fd76ef1b.REPO", "r") as f2:
	repo2 = json.load(f2)

for value in repo2:
	dupfound = 0
	for k in [	questitem_guid_name,
			melee_guid_name,
			pistol_guid_name,
			tool_guid_name,
			explosives_guid_name,
			sniperrifle_guid_name,
			assaultrifle_guid_name,
			shotgun_guid_name,
			suitcase_guid_name,
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
	else:
		continue

	if "Description" in value:
		if value["Description"].lower().startswith("outfit"):
			continue

	if "IsHitmanSuit" in value:
		continue

	if "ElusiveTarget" in value["CommonName"]:
		continue

	if value["CommonName"] == "RemoteExplosiveDuck Bundle":
		continue

	if "ghost" in value["CommonName"].lower() and \
		not "sieger" in value["CommonName"].lower():
		continue

	if "archme" in cn.lower() or 	\
		"archetype" in cn.lower() or \
		"selfie" in cn.lower():
		continue

	if "intelitem" in cn.lower() or 						\
		"Tool_McGuffin" in value["CommonName"] or 			\
		"Cassidy Video Tape" in value["CommonName"] or 		\
		"Constant Photography" in value["CommonName"] or 	\
		"Janus Letter" in value["CommonName"] or 			\
		"Janus Memoir" in value["CommonName"] or 			\
		"Microfilm Skunk" in value["CommonName"]:
		continue

	if value["ItemType"] == "eDetonator" and 							\
			not value["InventoryCategoryIcon"] == "tool" and 			\
			not value["InventoryCategoryIcon"] == "distraction" or 		\
			cn == "Mine Disarm Device" or 								\
			value["InventoryCategoryIcon"] == "remote":
		continue

	if 	value["HolsterAbility"] == "eHolsterQuestStorage" or 	\
		value["InventoryCategoryIcon"] == "QuestItem" or 		\
		value["InventoryCategoryName"] == "QuestItem" or 		\
		value["InventoryCategoryIcon"] == "questitem" or 		\
		value["InventoryCategoryName"] == "questitem" or 		\
	 	value["InventoryCategoryIcon"] == "key" or 				\
		value["InventoryCategoryName"] == "key" or 				\
		value["InventoryCategoryIcon"] == "USBStick" or 		\
		value["InventoryCategoryName"] == "USBStick" or 		\
		value["ActionRewardType"] == "AR_QuestItem" or 			\
		value["ItemType"] == "eOther_Keycard_A" or 				\
		"Chloroform Flask" in value["CommonName"] or 			\
		"Exploding_Watch_Battery" in value["CommonName"] or 	\
		"Key (Cultist Apartment)" in value["CommonName"] or 	\
		"QuestItem_" in value["CommonName"]:
		questitem_guid_name.append(["questitem", value["ID_"], cn])
		#questitems we might want to include for fun
		if value["InventoryCategoryIcon"] == "explosive" or		\
			value["InventoryCategoryName"] == "explosive":
			explosives_guid_name.append(["explosives", value["ID_"], cn])
		continue

	if "(Dartgun) Cure I" in cn or 	\
		"(Dartgun) Sick" in cn or 	\
		"(Dartgun) Tracker" in cn:
		continue
	
	if value["ItemType"] == "eCC_Bottle" and 					\
		not value["InventoryCategoryName"] == "explosive" and	\
		not value["CommonName"] == "Rat Poison":
		continue

	if "container" in value["InventoryCategoryIcon"].lower():
		continue

	if "outbreak" in cn.lower():
		continue

	#merge in anything we did not yet have
	#print("unk "+value["ID_"]+" "+cn)
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
	
	print("unk "+value["ID_"]+" "+cn)
	INVALID_CATEGORY_ICON_guid_name.append(["INVALID_CATEGORY_ICON", value["ID_"], cn])




out = open("Repository.c", "w")

out.write("#ifndef REPOSITORY_C\n")
out.write("#define REPOSITORY_C\n")
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
			suitcase_guid_name,
			distraction_guid_name,
			poison_guid_name,
			smg_guid_name,
			INVALID_CATEGORY_ICON_guid_name]: #,
			#ignored_guid_name]:
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

