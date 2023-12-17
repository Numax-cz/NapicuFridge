import {AnimationController} from "@ionic/angular";
import {animate, keyframes, state, style, transition, trigger} from "@angular/animations";

const animationCtrl = new AnimationController();


export const app_animation = ((_: HTMLElement, opts: any) => {
  // create root transition
  const rootTransition = animationCtrl
    .create()
    .duration(opts.duration || 200)
    .easing('cubic-bezier(0.7,0,0.3,1)');

  const enterTransition = animationCtrl.create().addElement(opts.enteringEl);
  const exitTransition = animationCtrl.create().addElement(opts.leavingEl);

  enterTransition.fromTo('opacity', '0', '1');
  exitTransition.fromTo('opacity', '1', '0');

  let value: number = 2.5;

  if (opts.direction === 'forward') {
    enterTransition.fromTo('transform', `translateY(${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateX(0%)', `translateY(-${value}%)`);
  } else {
    enterTransition.fromTo('transform', `translateY(-${value}%)`, 'translateY(0%)');
    exitTransition.fromTo('transform', 'translateY(0%)', `translateY(${value}%)`);
  }

  rootTransition.addAnimation([enterTransition, exitTransition]);
  return rootTransition;
});


export const alert_animations: any[] = [
  //Programování animací skrze Angular
  trigger('AlertAnimation', [
    state('void', style({ opacity: 0, transform: 'translate(-50%, 200%)' })),
    transition(':enter', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 0%)' }))
    ]),
    transition(':leave', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 200%)' }))
    ]),
  ]),

  trigger('AlertAnimationTop', [
    state('void', style({ opacity: 0, width: "0%", transform: 'translate(-50%, -200%)' })),
    transition(':enter', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, 0%)' })),
      animate('200ms ease-in-out', style({ width: '80%' }))
    ]),
    transition(':leave', [
      animate(200, style({ opacity: 1, transform: 'translate(-50%, -200%)' }))
    ]),
  ]),

  trigger('AlertAnimationBackground', [
    state('void', style({ opacity: 0 })),
    transition(':enter', [
      animate(150, style({ opacity: 1 }))
    ]),
    transition(':leave', [
      animate(150, style({ opacity: 0 }))
    ]),
  ]),
]

export const show_box_animations: any[] = [
  // trigger('ShowBoxAnimation', [
  //   state('void', style({ height: "0" })),
  //   transition(':enter', [
  //     animate(150, style({ height: "0" }))
  //   ]),
  //   transition(':leave', [
  //     animate(150, style({ height: "100%" }))
  //   ]),
  // ]),

  //TODO VARIABLE
  trigger('InfoBoxRotatedState', [
    state('false', style({ transform: 'rotate(0)' })),
    state('true', style({ transform: 'rotate(90deg)' })),
    transition('true => false', animate('150ms ease-out')),
    transition('false => true', animate('150ms ease-in'))
  ]),
  //TODO VARIABLE
  trigger('InfoBoxRotatedStateMinus', [
    state('false', style({ transform: 'rotate(0)' })),
    state('true', style({ transform: 'rotate(-90deg)' })),
    transition('true => false', animate('150ms ease-out')),
    transition('false => true', animate('150ms ease-in'))
  ])
]

export const arrows_expand_animations: any[] = [



  trigger('ArrowExpand', [
    state('void', style({ transform: 'rotate(0)'})),
    transition(':enter', [
      animate(150, style({ transform: 'rotate(90deg)'}))
    ]),
  ]),

]

export const favourite_color_animations: any[] = [
  //Programování animací skrze Angular
    trigger('FavouriteColorDeleteAnimation', [
      transition('true => false, false => true', [
        animate(
          300,
          keyframes([
            style({ rotate: '0deg' }),
            style({
              rotate: '3.5deg',

            }),

            style({
              rotate: '-3.5deg',
            }),

            style({ rotate: '0deg' }),
          ])
        ),
      ]),
      state('void', style({opacity: 0, transform: "translateY(-100%)"})),
      transition(':enter', [
        animate(150, style({opacity: 1,  transform: "translateY(0)" }))
      ]),
      transition(':leave', [
        animate(150, style({ opacity: 0, transform: "translateY(-50%)" }))
      ]),
  ]),

  trigger('FavouriteColorScale', [
    state('false', style({ scale: '1' })),
    state('true', style({ scale: '1.25' })),
    transition('true => false', animate('150ms ease-out')),
    transition('false => true', animate('150ms ease-in'))
  ])
]

