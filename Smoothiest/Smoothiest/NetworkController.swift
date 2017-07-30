//
//  NetworkController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import Foundation

class NetworkController: NSObject {
    static func getIngredientAmountsFromServer(ingredients: [Ingredient], smoothieAmountInOz: Float, completion: @escaping (_ results: [String:Any]) -> Void) {
//        for _ in ingredients {
//            amounts.append(NetworkController.generateRandomAmount())
//        }
        
        let urlString = URL(string: "http://demo.smoothiest.tech/debug.php")
        if let url = urlString {
            let task = URLSession.shared.dataTask(with: url) { (data, response, error) in
                if error != nil {
                    print(error ?? "Network call error was nil")
                } else {
                    if let usableData = data {
                        do {
                            if let res = try JSONSerialization.jsonObject(with: usableData, options: []) as? [String:Any] {
                                completion(res)
                            }
                        } catch {
                            print(error)
                        }
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
