//
//  FirstViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class FirstViewController: UIViewController {

    @IBOutlet weak var amountField: UITextField!
    @IBOutlet weak var strawberrySwitch: UISwitch!
    @IBOutlet weak var mangoSwitch: UISwitch!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }

    @IBAction func gimmeASmoothiePressed(_ sender: Any) {
        var ingredients = [Ingredient]()
        let smoothieAmount = Float(amountField.text!)
        if strawberrySwitch.isOn {
            ingredients.append(Ingredient(name: "strawberries"))
        }
        if mangoSwitch.isOn {
            ingredients.append(Ingredient(name: "mangoes"))
        }
        guard userErrorCheck(ingredients: ingredients, smoothieAmount: smoothieAmount) == true else {
            return
        }
        print(NetworkController.getIngredientAmountsFromServer(ingredients: ingredients, smoothieAmountInOz: smoothieAmount!))
    }
    
    func userErrorCheck(ingredients: [Ingredient], smoothieAmount: Float?) -> Bool {
        guard ingredients.count > 0 else {
            print("No ingredients selected")
            return false
        }
        guard smoothieAmount != nil else {
            print("Non-number detected in smoothie amount field")
            return false
        }
        let unwrappedSmoothieAmount = smoothieAmount!
        guard unwrappedSmoothieAmount > 0 else {
            print("Smoothie amount field must be greater than zero")
            return false
        }
        return true
    }
}
