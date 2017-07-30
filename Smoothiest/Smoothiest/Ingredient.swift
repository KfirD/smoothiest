//
//  Ingredient.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import Foundation
import UIKit

class Ingredient: NSObject {
    var name: String
    var image: UIImage
    
    init(_ name: String, image: UIImage) {
        self.name = name
        self.image = image
    }
}
