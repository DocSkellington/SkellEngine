File hierarchy          {#file_hierarchy}
========
[TOC]

# Hierarchy {#file_hierarchy_graph}
Here is an example of a file hierarchy tree. The folders in red are the folders that contain "default" data for the game.

![File hierarchy tree](hierarchyTree.png "File hierarchy")

# Explanation {#file_hierarchy_explanation}
You can define global data in the `media` folder. These data will be used as a base for every level and instance that needs them.

If a new declaration is found in the level-specific folder, then the new values override the ones from the root.

For entities, you can also define instance-specific data. For instance, you'd like to set a specific enemy at a precise position; not all of the enemies at the same starting position.