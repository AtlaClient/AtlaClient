AtlaClient Test Plugin

Installation:
- Make subfolder in "%atlaclient%\plugins" (i.e. "testplugin")
- Copy testplugin.dll and readme.txt there.

Plugin features:

1. Plugin's menu items:
1.1. Mana balance (only for MagicDeep)
Check mana balance for all local factions.
Check MGEN mana income, monster mana maintenance, use mana for cast.
To activate this feature, add "EAPMagicDeep=1" key to
[PluginsStr] section of options.txt

1.2. Run AutoStudy.
To use this, supply following commentary order for target units.

@; study list_of_skills_and_levels
example: @; study suba,5,drag,5

When Run AutoStudy item selected, plugin searches for local units with 
these comments and no monthlong orders. Then, plugin checks an 
opportunity of studying of required skills (by turns) and adds STUDY 
order if applicable. If skill is already learned, it will be removed 
from the list. If other skills are necessary to study target skill, 
the prerequisite skills will be learned first.

1.3. Scan faction addresses.
Plugin scans all events of local factions for strings like this:

"The address of FACTION (23) is mail@m..."

Found adresses are stored in client database.

1.4. Run AutoAttack.
To use this, supply following commentary order for target units.

@; attack list_of_faction_numbers
example: @; attack 2

When Run AutoAttack item selected, plugin searches for local units with 
these comments and no ATTACK orders. Then, plugin checks region for unit
of these factions and add ATTACK order.

1.5. Fix turn blocks order.
This command scans all units orders and:
1) convert to CUSTOM all orders within TURN-ENDTURN blocks
and removing its commented flag from commented nonrepeating orders.
2) convert from CUSTOM all orders outside TURN-ENDTURN blocks,
except orders with "faction xx new yy".

2.Plugins\Config menu items:
2.1. Test Plugin
Just add event with plugin file name.

3. Function for expressions.
3.1. weather()
Function returns 0, if weather will be "Normal", otherwise returns 1.

4. Optional support for non-standard maintenance and study fees.
4.1 MagicDeep style:
- additional cost for studying additional skills for nonleaders.
- +2 level bonus for studying one skill for nonleaders.
- movement bonus from magic roads.
- different movepoints for ship (with windstone and summon wind support).
- light and air ships slows by bad weather by 3, instead of 2.
To activate this feature, add "EAPMagicDeep=1" key to
[PluginsStr] section of options.txt
PS. 40$ leaders' maintenance now supported in AtlaClient.
4.2 Grelth style:
- extra maintenance payments: $3 per skill level.
To activate this feature, add "EAPGrelth=1" key to
[PluginsStr] section of options.txt
PS. Base maintenace: $30 for leaders, $5 for others now supported
in AtlaClient, so plugin use AtlaClients settings.
4.3 Tarmellion style:
- maintenance decrease: $2 per skill "SCOU" level.
- different movepoints for ship (with summon wind support).
To activate this feature, add "EAPTarmellion=1" key to
[PluginsStr] section of options.txt
