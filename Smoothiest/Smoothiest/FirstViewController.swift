//
//  FirstViewController.swift
//  Smoothiest
//
//  Created by Keegan Papakipos on 7/29/17.
//  Copyright © 2017 kpapakipos. All rights reserved.
//

import UIKit

class FirstViewController: UIViewController {

    @IBOutlet weak var strawberrySwitch: UISwitch!
    @IBOutlet weak var mangoSwitch: UISwitch!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }

    @IBAction func gimmeASmoothiePressed(_ sender: Any) {
        var ingredients = [Ingredient]()
        if strawberrySwitch.isOn {
            ingredients.append(Ingredient(name: "strawberries"))
        }
        if mangoSwitch.isOn {
            ingredients.append(Ingredient(name: "mangoes"))
        }
        NetworkController.sendIngredientsToServer(ingredients: ingredients)
    }

}

