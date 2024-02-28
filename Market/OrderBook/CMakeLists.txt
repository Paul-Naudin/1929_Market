# Ajouter les sources de la bibliothèque OrderBook
add_library(OrderBook OrderBook.h OrderBook.cpp Order.h Order.cpp IOrder.h)

# Ajouter le chemin d'inclusion du dossier OrderBook
target_include_directories(OrderBook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
