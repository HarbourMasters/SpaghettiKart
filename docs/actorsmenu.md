@page actorsmenu Actors

Actors are dynamic game objects usually subject to game physics such as gravity, force, and collision. They might move around the map, spin, or interactive in some manner. Alternatively, static game objects cannot move and cannot be interacted with. They can use collision but not physics. A track model and Mario Raceway's Pipe are examples of static objects. Any of the items or player karts are examples of dynamic game objects; actors.  

mk64 contains two systems for implementing actors. They have been deemed as actors and objects, albeit an explicit or categorical difference between the two has yet to be determined. The running theory is that the systems were designed by different developers that were perhaps not communicating resulting in game objects being randomly distributed between the two.  

The two main variables to concern yourself with are `gActorList` and `gObjectList`; the core containers of the two systems.  

@ref gActorList contains:
```
trees, bushes, falling rocks, kiwano-fruit, banana, shells, piranha-plant, train wheels, rail-road crossing, cows, yoshi-valley's egg, spinning signs, palm trees, paddle for the paddle-wheeled boat, wheels for cars/trucks, etc.
```
The actor code has two main parts; the update code and the render code. The update code alters the position/rotation of the object whereas the render code displays the object on the screen.  

@ref gObjectList contains:
```
thwomps, bowser fire-breath, moles, hedgehogs, snowmen, crabs, particles, leaves, etc (also, probably penguins and birds).
```
The object system is much more complex than actors and is not well documented.
@htmlonly
<br>
<div class="pagebutton">
<a class="pagea" href="actors.html">
<div class="pagelink">
  <div class="pageimg"><img width=320 src="buttonimage.png" /></div>
  <div class="content">
    <div class="pageheading">Actors</div>
    <div class="pagedescription">
      <p>Some actor stuff</p>
    </div>
  </div>
</div>
</a>
</div>

<div class="pagebutton">
<a class="pagea" href="objects.html">
<div class="pagelink">
  <div class="pageimg"><img width=320 src="buttonimage.png" /></div>
  <div class="content">
    <div class="pageheading">Objects</div>
    <div class="pagedescription">
      <p>Some object stuff</p>
    </div>
  </div>
</div>
</a>
</div>

<div class="pagebutton">
<a class="pagea" href="vehicles.html">
<div class="pagelink">
  <div class="pageimg"><img width=320 src="buttonimage.png" /></div>
  <div class="content">
    <div class="pageheading">Vehicles</div>
    <div class="pagedescription">
      <p>Trains, boats, cars, trucks, and more.</p>
    </div>
  </div>
</div>
</a>
</div>

<div class="pagebutton">
<a class="pagea" href="items.html">
<div class="pagelink">
  <div class="pageimg"><img width=320 src="buttonimage.png" /></div>
  <div class="content">
    <div class="pageheading">Items</div>
    <div class="pagedescription">
      <p>Item Pickups</p>
    </div>
  </div>
</div>
</a>
</div>

@endhtmlonly

@subpage actors
@subpage objects
@subpage vehicles
@subpage items
