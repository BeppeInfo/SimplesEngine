remaining_game_time_ms = getNumberValue("remaining_game_time_ms")
remaining_game_time_ms -= elapsedTime()
remaining_game_time = remaining_game_time_ms / 1000
game_time = getNumberValue("total_game_time") - remaining_game_time
game_points = getNumberValue("game_points")
num_items = getNumberValue("num_items")

works( "updates", 2 )

if remaining_game_time_ms <= 0:
    
   endGame()
   
else:

   registerValue( 2 )
   registerValue( actorsNumber() )
   registerValue( game_time )
    
   for id in range( actorsNumber() ):

      registerObjIdPos(id)
        
      if isCharId(id):

         if objIdCollision(id):
             
             endGame()
             break

      else:

         if objIdCollision(id):
             
            deleteObjId(id)
            game_points + 1

            if game_points >= num_items:
               endGame()

            break

setNumberValue("remaining_game_time_ms", remaining_game_time_ms)
setNumberValue("remaining_game_time", remaining_game_time)
setNumberValue("game_time", game_time)
setNumberValue("game_points", game_points)
