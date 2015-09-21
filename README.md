The House That ASCII Built
==========================

https://www.reddit.com/r/dailyprogrammer/comments/3ltee2/20150921_challenge_233_easy_the_house_that_ascii/

Translates "blueprints," e.g.:

    7
    ***                    ***
    ***     **  ***  **    ***
    ***   ***************  ***
    ***   ***************  ***
    ***   ***************  ***
    **************************
    **************************

into "houses" (with one random ground-floor door, random windows, and steepled
roofs), like so:

          A                                                                                           A      
         / \                                                                                         / \     
        /   \                                             A                                         /   \    
       /     \                                           / \                                       /     \   
      /       \                         A               /   \               A                     /       \  
     /         \                       / \             /     \             / \                   /         \ 
    +-----------+               A     /   \     A     /       \     A     /   \     A           +-----------+
    | o   o     |              / \   /     \   / \   /         \   / \   /     \   / \          | o         |
    |           |             /   \ +-------+ /   \ +-----------+ /   \ +-------+ /   \         |           |
    | o         |            /     \|     o |/     \|         o |/     \| o     |/     \        | o       o |
    |           |     A     +-------+       +-------+           +-------+       +-------+       |           |
    |           |    / \    |         o   o   o       o       o   o               o   o |       |         o |
    |           |   /   \   |                                                           |   A   |           |
    | o         |  /     \  | o   o           o   o   o               o   o           o |  / \  | o         |
    |           | /       \ |                                                           | /   \ |           |
    | o   o     |/         \| o           o   o       o   o   o       o   o   o   o     |/     \| o   o   o |
    |           +-----------+                                                           +-------+           |
    |             o       o   o           o   o           o   o   o   o   o               o   o       o     |
    |                                                                                                       |
    |                 o                   o               o      | |      o   o       o       o       o     |
    +-------------------------------------------------------------------------------------------------------+

A number of assumptions are made; in particular the bottom floor is continuous
and there are no "balconies."
