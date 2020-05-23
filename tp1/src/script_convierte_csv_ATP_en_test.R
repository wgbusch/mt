library(dplyr)
atp2019 <- read.csv("Desktop/tennis_atp/atp_matches_2019.csv")
atp2019 <- select(atp2019 , "winner_name", "loser_name")
atp2019 <- mutate(atp2019, id = 1, winner_points = 1, loser_points = 0)
atp2019 <- atp2019[c(3,1,4,2,5)]
atp2019 <- arrange(atp2019, winner_name)
perdedores_atp_2019 <- names(table(atp2019$loser_name))
perdedores_atp_2019 <- tibble(perdedores_atp_2019)
ganadores_atp_2019 <- names(table(atp2019$winner_name))
ganadores_atp_2019 <- tibble(ganadores_atp_2019)
names(ganadores_atp_2019)[c(1)] <- "nombres"
names(perdedores_atp_2019)[c(1)] <- "nombres"
jugadores_atp_2019 <- bind_rows(ganadores_atp_2019,perdedores_atp_2019)
jugadores_atp_2019 <- names(table(jugadores_atp_2019$nombres))
jugadores_atp_2019 <- tibble(jugadores_atp_2019)
cant_jugadores <- dim(jugadores_atp_2019)[1]
jugadores_atp_2019 <- mutate(jugadores_atp_2019, num = {1:cant_jugadores})
names(jugadores_atp_2019)[c(1)] <- "loser_name"
names(jugadores_atp_2019)[c(2)] <- "loser_num"
x = left_join(atp2019,jugadores_atp_2019)
names(jugadores_atp_2019)[c(1)] <- "winner_name"
names(jugadores_atp_2019)[c(2)] <- "winner_num"
y = left_join(x,jugadores_atp_2019)
y <- y[c(1,7,3,6,5)]
names(y) <- NULL #elimina los headers
cant_partidos <- dim(y)[1] 
write.table(y, file = "Documents/mt/src/Tests_ATP/ATP_2019.in", row.names = FALSE) # guarda un archivo txt
z = list(cant_jugadores, cant_partidos)
write.table(z, file = "Documents/mt/src/Tests_ATP/info_2019.in", row.names = FALSE)
