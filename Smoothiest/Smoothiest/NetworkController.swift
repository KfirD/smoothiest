//
//  NetworkController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import Foundation

class NetworkController: NSObject {
    static func getIngredientAmountsFromServer(ingredients: [Ingredient], smoothieAmountInOz: Float) -> [Float] {
        var amounts = [Float]()
        for _ in ingredients {
            amounts.append(NetworkController.generateRandomAmount())
        }
        return amounts
        
        let urlString = URL(string: "smoothiest.tech")
        if let url = urlString {
            let task = URLSession.shared.dataTask(with: url) { (data, response, error) in
                if error != nil {
                    print(error ?? "Network call error was nil")
                } else {
                    if let usableData = data {
                        print(usableData) //JSONSerialization
                    }
                }
            }
            task.resume()
        }
    }
    static func generateRandomAmount() -> Float {
        return Float(arc4random_uniform(1000))/Float(100)
    }
}
