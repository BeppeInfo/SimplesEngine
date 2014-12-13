# -*- coding: cp1252 -*-
remaining_game_time_ms -= elapsedTime()
game_time = total_game_time - (remaining_game_time_ms / 1000)
game_points = getNumberValue("game_points")

if remaining_game_time_ms <= 0:
  
   endGame("game_over")
   
else:

   #registerValue( clientKeep )
   #registerValue( actorsNumber() )
   #registerValue( game_time )
    
   for id in range( actorsNumber() ):

      x,y = getObjIdPos(id)
      #registerValue( x )
      #registerValue( y ) 
        
      if isCharId(id):

         if objIdCollision(playerId, id):
 
            endGame("credits")
            break

         elif id != playerId:

            dx = getObjIdPos(playerId)[0] - x
            dy = getObjIdPos(playerId)[1] - y

            if math.fabs(dx) > math.fabs(dy):
               if dx > 0: setCharId( id, WR )
               else: setCharId( id, WL )
            else:
               if dy > 0: setCharId( id, WD )
               else: setCharId( id, WU ) 

      else:

         if objIdCollision(playerId, id):
             
            deleteObjId(id)
            game_points += 1

            if game_points >= num_items:
               endGame("game_over")

            break

setNumberValue("remaining_game_time", remaining_game_time_ms / 1000)
setNumberValue("game_time", game_time)
setNumberValue("game_points", game_points)

